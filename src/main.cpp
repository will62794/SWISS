#include "logic.h"
#include "contexts.h"
#include "model.h"
#include "grammar.h"
#include "expr_gen_smt.h"
#include "benchmarking.h"
#include "bmc.h"
#include "enumerator.h"
#include "utils.h"
#include "synth_loop.h"
#include "sat_solver.h"
#include "wpr.h"

#include <iostream>
#include <iterator>
#include <string>
#include <cstdlib>

using namespace std;

bool do_invariants_imply_conjecture(shared_ptr<ConjectureContext> conjctx) {
  smt::solver& solver = conjctx->ctx->solver;
  return !solver.check_sat();
}

bool is_redundant(
    shared_ptr<InvariantsContext> invctx,
    shared_ptr<Value> formula)
{
  smt::solver& solver = invctx->ctx->solver;
  solver.push();
  solver.add(invctx->e->value2expr(shared_ptr<Value>(new Not(formula))));

  bool res = !solver.check_sat();
  solver.pop();
  return res;
}

value augment_invariant(value a, value b) {
  if (Forall* f = dynamic_cast<Forall*>(a.get())) {
    return v_forall(f->decls, augment_invariant(f->body, b));
  }
  else if (Or* o = dynamic_cast<Or*>(a.get())) {
    vector<value> args = o->args;
    args.push_back(b);
    return v_or(args);
  }
  else {
    return v_or({a, b});
  }
}

void enumerate_next_level(
    vector<value> const& fills,
    vector<value>& next_level,
    value invariant,
    QuantifierInstantiation const& qi)
{
  for (value fill : fills) {
    if (eval_qi(qi, fill)) {
      value newv = augment_invariant(invariant, fill);
      next_level.push_back(newv);
    }
  }
}

void print_wpr(shared_ptr<Module> module, int count)
{
  shared_ptr<Action> action = shared_ptr<Action>(new ChoiceAction(module->actions));
  value conj = v_and(module->conjectures);
  cout << "conjecture: " << conj->to_string() << endl;

  vector<value> all_conjs;

  value w = conj;
  all_conjs.push_back(w);
  for (int i = 0; i < count; i++) {
    w = wpr(w, action)->simplify();
    all_conjs.push_back(w);
  }

  /*cout << "list:" << endl;
  for (value conj : all_conjs) {
    for (value part : aggressively_split_into_conjuncts(conj)) {
      cout << part->to_string() << endl;
    }
  }
  cout << endl;*/

  cout << "wpr: " << w->to_string() << endl;

  if (is_itself_invariant(module, all_conjs)) {
  //if (is_wpr_itself_inductive(module, conj, count)) {
    printf("yes\n");
  } else{
    printf("no\n");
  }
}

int run_id;
extern bool enable_smt_logging;

int main(int argc, char* argv[]) {
  std::istreambuf_iterator<char> begin(std::cin), end;
  std::string json_src(begin, end);

  shared_ptr<Module> module = parse_module(json_src);

  cout << "conjectures:" << endl;
  for (value v : module->conjectures) {
    cout << v->to_string() << endl;
  }

  srand((int)time(NULL));
  run_id = rand();

  Options options;
  options.enum_sat = false;
  options.with_conjs = false;
  options.whole_space = false;
  options.pre_bmc = false;
  options.post_bmc = false;
  options.get_space_size = false;
  options.minimal_models = false;

  EnumOptions enum_options;
  enum_options.arity = -1;
  enum_options.depth = -1;
  enum_options.conj = false;
  enum_options.conj_arity = -1;
  enum_options.disj_arity = -1;
  enum_options.impl_shape = false;
  //options.strat2 = false;
  enum_options.strat_alt = false;

  int seed = 1234;
  bool check_inductiveness = false;
  bool check_rel_inductiveness = false;
  bool check_implication = false;
  bool incremental = false;
  bool breadth = false;
  bool finisher = false;
  bool wpr = false;
  int wpr_index = 0;
  for (int i = 1; i < argc; i++) {
    if (argv[i] == string("--random")) {
      seed = (int)time(NULL);
    }
    else if (argv[i] == string("--seed")) {
      assert(i + 1 < argc);
      seed = atoi(argv[i+1]);
      i++;
    }
    else if (argv[i] == string("--wpr")) {
      assert(i + 1 < argc);
      wpr_index = atoi(argv[i+1]);
      wpr = true;
      i++;
    }
    else if (argv[i] == string("--check-inductiveness")) {
      check_inductiveness = true;
    }
    else if (argv[i] == string("--check-rel-inductiveness")) {
      check_rel_inductiveness = true;
    }
    else if (argv[i] == string("--check-implication")) {
      check_implication = true;
    }
    else if (argv[i] == string("--finisher")) {
      finisher = true;
    }
    else if (argv[i] == string("--incremental")) {
      incremental = true;
    }
    else if (argv[i] == string("--breadth")) {
      breadth = true;
    }
    else if (argv[i] == string("--whole-space")) {
      options.whole_space = true;
    }
    else if (argv[i] == string("--arity")) {
      assert(i + 1 < argc);
      enum_options.arity = atoi(argv[i+1]);
      i++;
    }
    else if (argv[i] == string("--depth")) {
      assert(i + 1 < argc);
      enum_options.depth = atoi(argv[i+1]);
      i++;
    }
    else if (argv[i] == string("--conj")) {
      enum_options.conj = true;
    }
    else if (argv[i] == string("--conj-arity")) {
      assert(i + 1 < argc);
      enum_options.conj_arity = atoi(argv[i+1]);
      i++;
    }
    else if (argv[i] == string("--disj-arity")) {
      assert(i + 1 < argc);
      enum_options.disj_arity = atoi(argv[i+1]);
      i++;
    }
    else if (argv[i] == string("--enum-sat")) {
      options.enum_sat = true;
    }
    else if (argv[i] == string("--impl-shape")) {
      enum_options.impl_shape = true;
    }
    else if (argv[i] == string("--with-conjs")) {
      options.with_conjs = true;
    }
    //else if (argv[i] == string("--strat2")) {
    //  options.strat2 = true;
    //}
    else if (argv[i] == string("--strat-alt")) {
      enum_options.strat_alt = true;
    }
    else if (argv[i] == string("--log-smt-files")) {
      enable_smt_logging = true;
    }
    else if (argv[i] == string("--pre-bmc")) {
      options.pre_bmc = true;
    }
    else if (argv[i] == string("--post-bmc")) {
      options.post_bmc = true;
    }
    else if (argv[i] == string("--get-space-size")) {
      options.get_space_size = true;
    }
    else if (argv[i] == string("--minimal-models")) {
      options.minimal_models = true;
    }
    else {
      cout << "unreocgnized argument " << argv[i] << endl;
      return 1;
    }
  }

  if (wpr) {
    print_wpr(module, wpr_index);
    return 0;
  }

  if (check_inductiveness) {
    printf("just checking inductiveness...\n");
    if (is_itself_invariant(module, module->conjectures)) {
      printf("yes\n");
    } else{
      printf("no\n");
    }
    return 0;
  }

  if (check_rel_inductiveness) {
    printf("just inductiveness of the last one...\n");
    value v = module->conjectures[module->conjectures.size() - 1];
    vector<value> others;
    for (int i = 0; i < (int)module->conjectures.size() - 1; i++) {
      others.push_back(module->conjectures[i]);
    }
    if (is_invariant_wrt(module, v_and(others), v)) {
      printf("yes\n");
    } else{
      printf("no\n");
    }
    return 0;
  }


  if (check_implication) {
    printf("just checking inductiveness...\n");
    vector<value> vs;
    for (int i = 0; i < (int)module->conjectures.size(); i++) {
      vs.push_back(i == 0 ? v_not(module->conjectures[i]) :
          module->conjectures[i]);
    }
    if (is_satisfiable(module, v_and(vs))) {
      printf("first is NOT implied by the rest\n");
    } else{
      printf("first IS implied by the rest\n");
    }
    return 0;
  }

  printf("random seed = %d\n", seed);
  srand(seed);

  assert(!(breadth && incremental));
  assert(!(breadth && finisher));
  assert(!(incremental && finisher));

  if (finisher) {
    printf("strategy: finisher\n");
  } else if (incremental) {
    printf("strategy: incremental\n");
  } else {
    printf("strategy: breadth\n");
  }

  try {
    assert(argc >= 3);

    if (breadth) {
      synth_loop_incremental_breadth(module, enum_options, options);
    } else if (incremental) {
      synth_loop_incremental(module, enum_options, options);
    } else {
      synth_loop(module, enum_options, options);
    }

    return 0;
  } catch (z3::exception exc) {
    printf("got z3 exception: %s\n", exc.msg());
    throw;
  }
}
