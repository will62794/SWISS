#include "logic.h"
#include "lib/json11/json11.hpp"
#include "benchmarking.h"

#include <cassert>
#include <iostream>

using namespace std;
using namespace json11;

shared_ptr<Module> json2module(Json j);
vector<string> json2string_array(Json j);
vector<shared_ptr<Value>> json2value_array(Json j);
vector<shared_ptr<Action>> json2action_array(Json j);
vector<shared_ptr<Action>> json2action_array_from_map(Json j);
vector<shared_ptr<Sort>> json2sort_array(Json j);
vector<VarDecl> json2decl_array(Json j);
shared_ptr<Value> json2value(Json j);
shared_ptr<Sort> json2sort(Json j);
shared_ptr<Action> json2action(Json j);

shared_ptr<Module> parse_module(string const& src) {
  string err;
  Json j = Json::parse(src, err);
  return json2module(j);
}

shared_ptr<Module> json2module(Json j) {
  assert(j.is_object());
  return shared_ptr<Module>(new Module(
      json2string_array(j["sorts"]),
      json2decl_array(j["functions"]),
      json2value_array(j["axioms"]),
      json2value_array(j["inits"]),
      json2value_array(j["conjectures"]),
      json2value_array(j["templates"]),
      json2action_array_from_map(j["actions"])));
}

vector<string> json2string_array(Json j) {
  assert(j.is_array());
  vector<string> res;
  for (Json elem : j.array_items()) {
    assert(elem.is_string());
    res.push_back(elem.string_value());
  }
  return res;
}

vector<shared_ptr<Value>> json2value_array(Json j) {
  assert(j.is_array());
  vector<shared_ptr<Value>> res;
  for (Json elem : j.array_items()) {
    res.push_back(json2value(elem));
  }
  return res;
}

vector<shared_ptr<Action>> json2action_array(Json j) {
  assert(j.is_array());
  vector<shared_ptr<Action>> res;
  for (Json elem : j.array_items()) {
    res.push_back(json2action(elem));
  }
  return res;
}

vector<shared_ptr<Action>> json2action_array_from_map(Json j) {
  assert(j.is_object());
  vector<shared_ptr<Action>> res;
  for (auto p : j.object_items()) {
    res.push_back(json2action(p.second));
  }
  return res;
}

vector<shared_ptr<Sort>> json2sort_array(Json j) {
  assert(j.is_array());
  vector<shared_ptr<Sort>> res;
  for (Json elem : j.array_items()) {
    res.push_back(json2sort(elem));
  }
  return res;
}

vector<VarDecl> json2decl_array(Json j) {
  assert(j.is_array());
  vector<VarDecl> res;
  for (Json elem : j.array_items()) {
    assert(elem.is_array());
    assert(elem.array_items().size() == 3);
    assert(elem[0] == "const" || elem[0] == "var");
    assert(elem[1].is_string());
    res.push_back(VarDecl(elem[1].string_value(), json2sort(elem[2])));
  }
  return res;
}

shared_ptr<Value> json2value(Json j) {
  assert(j.is_array());
  assert(j.array_items().size() >= 1);
  assert(j[0].is_string());
  string type = j[0].string_value();
  if (type == "forall") {
    assert (j.array_items().size() == 3);
    return shared_ptr<Value>(new Forall(json2decl_array(j[1]), json2value(j[2])));
  }
  else if (type == "exists") {
    assert (j.array_items().size() == 3);
    return shared_ptr<Value>(new Exists(json2decl_array(j[1]), json2value(j[2])));
  }
  else if (type == "var") {
    assert (j.array_items().size() == 3);
    assert (j[1].is_string());
    return shared_ptr<Value>(new Var(j[1].string_value(), json2sort(j[2])));
  }
  else if (type == "const") {
    assert (j.array_items().size() == 3);
    assert (j[1].is_string());
    return shared_ptr<Value>(new Const(j[1].string_value(), json2sort(j[2])));
  }
  else if (type == "implies") {
    assert (j.array_items().size() == 3);
    return shared_ptr<Value>(new Implies(json2value(j[1]), json2value(j[2])));
  }
  else if (type == "eq") {
    assert (j.array_items().size() == 3);
    return shared_ptr<Value>(new Eq(json2value(j[1]), json2value(j[2])));
  }
  else if (type == "not") {
    assert (j.array_items().size() == 2);
    return shared_ptr<Value>(new Not(json2value(j[1])));
  }
  else if (type == "apply") {
    assert (j.array_items().size() == 3);
    return shared_ptr<Value>(new Apply(json2value(j[1]), json2value_array(j[2])));
  }
  else if (type == "and") {
    assert (j.array_items().size() == 2);
    return shared_ptr<Value>(new And(json2value_array(j[1])));
  }
  else if (type == "or") {
    assert (j.array_items().size() == 2);
    return shared_ptr<Value>(new Or(json2value_array(j[1])));
  }
  else if (type == "__wild") {
    assert (j.array_items().size() == 1);
    return shared_ptr<Value>(new TemplateHole());
  }
  else {
    printf("value type: %s\n", type.c_str());
    assert(false && "unrecognized Value type");
  }
}

shared_ptr<Sort> json2sort(Json j) {
  assert(j.is_array());
  assert(j.array_items().size() >= 1);
  assert(j[0].is_string());
  string type = j[0].string_value();
  if (type == "booleanSort") {
    return shared_ptr<Sort>(new BooleanSort());
  }
  else if (type == "uninterpretedSort") {
    assert (j.array_items().size() == 2);
    return shared_ptr<Sort>(new UninterpretedSort(j[1].string_value()));
  }
  else if (type == "functionSort") {
    return shared_ptr<FunctionSort>(
      new FunctionSort(json2sort_array(j[1]), json2sort(j[2])));
  }
  else {
    assert(false && "unrecognized Sort type");
  }
}

shared_ptr<Action> json2action(Json j) {
  assert(j.is_array());
  assert(j.array_items().size() >= 1);
  assert(j[0].is_string());
  string type = j[0].string_value();
  if (type == "localAction") {
    assert(j.array_items().size() == 3);
    return shared_ptr<Action>(new LocalAction(json2decl_array(j[1]), json2action(j[2])));
  }
  else if (type == "sequence") {
    assert(j.array_items().size() == 2);
    return shared_ptr<Action>(new SequenceAction(json2action_array(j[1])));
  }
  else if (type == "choice") {
    assert(j.array_items().size() == 2);
    return shared_ptr<Action>(new ChoiceAction(json2action_array(j[1])));
  }
  else if (type == "assume") {
    assert(j.array_items().size() == 2);
    return shared_ptr<Action>(new Assume(json2value(j[1])));
  }
  else if (type == "assign") {
    assert(j.array_items().size() == 3);
    return shared_ptr<Action>(new Assign(json2value(j[1]), json2value(j[2])));
  }
  else if (type == "if") {
    assert(j.array_items().size() == 3);
    return shared_ptr<Action>(new If(json2value(j[1]), json2action(j[2])));
  }
  else if (type == "ifelse") {
    assert(j.array_items().size() == 4);
    return shared_ptr<Action>(new IfElse(
        json2value(j[1]), json2action(j[2]), json2action(j[3])));
  }
  else {
    assert(false && "unrecognized Action type");
  }
}

string BooleanSort::to_string() const {
  return "bool";
}

string UninterpretedSort::to_string() const {
  return name;
}

string FunctionSort::to_string() const {
  string res = "(";
  for (int i = 0; i < domain.size(); i++) {
    if (i != 0) res += ", ";
    res += domain[i]->to_string();
  }
  return res + " -> " + range->to_string();
}

string Forall::to_string() const {
  string res = "forall ";
  for (int i = 0; i < decls.size(); i++) {
    if (i > 0) {
      res += ", ";
    }
    res += decls[i].name;
  }
  res += " . (" + body->to_string() + ")";
  return res;
}

string Exists::to_string() const {
  string res = "exists ";
  for (int i = 0; i < decls.size(); i++) {
    if (i > 0) {
      res += ", ";
    }
    res += decls[i].name;
  }
  res += " . (" + body->to_string() + ")";
  return res;
}

string Var::to_string() const {
  return name;
}

string Const::to_string() const {
  return name;
}

string Eq::to_string() const {
  return "(" + left->to_string() + ") = (" + right->to_string() + ")";
}

string Not::to_string() const {
  if (Eq* e = dynamic_cast<Eq*>(val.get())) {
    return "(" + e->left->to_string() + ") ~= (" + e->right->to_string() + ")";
  } else {
    return "~(" + val->to_string() + ")";
  }
}

string Implies::to_string() const {
  return "(" + left->to_string() + ") -> (" + right->to_string() + ")";
}

string And::to_string() const {
  string res = "";
  for (int i = 0; i < args.size(); i++) {
    if (i > 0) {
      res += " & ";
    }
    res += "(" + args[i]->to_string() + ")";
  }
  return res;
}

string Or::to_string() const {
  string res = "";
  for (int i = 0; i < args.size(); i++) {
    if (i > 0) {
      res += " | ";
    }
    res += "(" + args[i]->to_string() + ")";
  }
  return res;
}

string Apply::to_string() const {
  string res = func->to_string() + "(";
  for (int i = 0; i < args.size(); i++) {
    if (i > 0) {
      res += ", ";
    }
    res += args[i]->to_string();
  }
  return res + ")";
}

string TemplateHole::to_string() const {
  return "WILD";
}

value Forall::subst(string const& x, value e) const {
  return v_forall(decls, body->subst(x, e)); 
}

value Exists::subst(string const& x, value e) const {
  return v_exists(decls, body->subst(x, e)); 
}

value Var::subst(string const& x, value e) const {
  if (x == name) {
    return e;
  } else {
    return v_var(name, sort);
  }
}

value Const::subst(string const& x, value e) const {
  return v_const(name, sort);
}

value Eq::subst(string const& x, value e) const {
  return v_eq(left->subst(x, e), right->subst(x, e));
}

value Not::subst(string const& x, value e) const {
  return v_not(this->val->subst(x, e));
}

value Implies::subst(string const& x, value e) const {
  return v_implies(left->subst(x, e), right->subst(x, e));
}

value Apply::subst(string const& x, value e) const {
  vector<value> new_args;
  for (value const& arg : args) {
    new_args.push_back(arg->subst(x, e));
  }
  return v_apply(func->subst(x, e), move(new_args));
}

value And::subst(string const& x, value e) const {
  vector<value> new_args;
  for (value const& arg : args) {
    new_args.push_back(arg->subst(x, e));
  }
  return v_and(move(new_args));
}

value Or::subst(string const& x, value e) const {
  vector<value> new_args;
  for (value const& arg : args) {
    new_args.push_back(arg->subst(x, e));
  }
  return v_or(move(new_args));
}

value TemplateHole::subst(string const& x, value e) const {
  return v_template_hole();
}

value Forall::negate() const {
  return v_exists(decls, body->negate());
}

value Exists::negate() const {
  return v_forall(decls, body->negate());
}

value Var::negate() const {
  return v_not(v_var(name, sort));
}

value Const::negate() const {
  return v_not(v_const(name, sort));
}

value Eq::negate() const {
  return v_not(v_eq(left, right));
}

value Not::negate() const {
  return val;
}

value Implies::negate() const {
  return v_and({left, right->negate()});
}

value Apply::negate() const {
  return v_not(v_apply(func, args));
}

value And::negate() const {
  vector<value> new_args;
  for (value const& arg : args) {
    new_args.push_back(arg->negate());
  }
  return v_or(move(new_args));
}

value Or::negate() const {
  vector<value> new_args;
  for (value const& arg : args) {
    new_args.push_back(arg->negate());
  }
  return v_and(move(new_args));
}

value TemplateHole::negate() const {
  return v_not(v_template_hole());
}

uint64_t uid = 0;
string new_var_id() {
  return "__var." + to_string(uid++);
}

value Forall::uniquify_vars(map<string, string> const& m) const {
  map<string, string> new_m = m;
  vector<VarDecl> new_decls;
  for (VarDecl const& decl : this->decls) {
    string new_name = new_var_id();
    new_m[decl.name] = new_name;
    new_decls.push_back(VarDecl(new_name, decl.sort));
  }
  return v_forall(new_decls, body->uniquify_vars(new_m));
}

value Exists::uniquify_vars(map<string, string> const& m) const {
  map<string, string> new_m = m;
  vector<VarDecl> new_decls;
  for (VarDecl const& decl : this->decls) {
    string new_name = new_var_id();
    new_m[decl.name] = new_name;
    new_decls.push_back(VarDecl(new_name, decl.sort));
  }
  return v_forall(new_decls, body->uniquify_vars(new_m));
}

value Var::uniquify_vars(map<string, string> const& m) const {
  auto iter = m.find(this->name);
  assert(iter != m.end());
  return v_var(iter->second, this->sort);
}

value Const::uniquify_vars(map<string, string> const& m) const {
  return v_const(name, sort);
}

value Eq::uniquify_vars(map<string, string> const& m) const {
  return v_eq(left->uniquify_vars(m), right->uniquify_vars(m));
}

value Not::uniquify_vars(map<string, string> const& m) const {
  return v_not(this->val->uniquify_vars(m));
}

value Implies::uniquify_vars(map<string, string> const& m) const {
  return v_implies(left->uniquify_vars(m), right->uniquify_vars(m));
}

value Apply::uniquify_vars(map<string, string> const& m) const {
  vector<value> new_args;
  for (value const& arg : args) {
    new_args.push_back(arg->uniquify_vars(m));
  }
  return v_apply(func->uniquify_vars(m), move(new_args));
}

value And::uniquify_vars(map<string, string> const& m) const {
  vector<value> new_args;
  for (value const& arg : args) {
    new_args.push_back(arg->uniquify_vars(m));
  }
  return v_and(move(new_args));
}

value Or::uniquify_vars(map<string, string> const& m) const {
  vector<value> new_args;
  for (value const& arg : args) {
    new_args.push_back(arg->uniquify_vars(m));
  }
  return v_or(move(new_args));
}

value TemplateHole::uniquify_vars(map<string, string> const& m) const {
  return v_template_hole();
}

template <typename T>
void extend(vector<T> & a, vector<T> const& b) {
  for (T const& t : b) {
    a.push_back(t);
  }
}

void sort_decls(vector<VarDecl>& decls) {
  sort(decls.begin(), decls.end(), [](VarDecl const& a, VarDecl const& b) {
    string a_name, b_name;
    if (dynamic_cast<BooleanSort*>(a.sort.get())) {
      a_name = "";
    } else if (auto v = dynamic_cast<UninterpretedSort*>(a.sort.get())) {
      a_name = v->name;
    }
    if (dynamic_cast<BooleanSort*>(b.sort.get())) {
      b_name = "";
    } else if (auto v = dynamic_cast<UninterpretedSort*>(b.sort.get())) {
      b_name = v->name;
    }
    return a_name < b_name;
  });
}

value Forall::structurally_normalize_() const {
  value b = this->body->structurally_normalize_();
  if (Forall* inner = dynamic_cast<Forall*>(b.get())) {
    vector<VarDecl> new_decls = this->decls;
    extend(new_decls, inner->decls);
    sort_decls(new_decls);
    return v_forall(new_decls, inner->body);
  } else {
    return v_forall(this->decls, b);
  }
}

value Exists::structurally_normalize_() const {
  value b = this->body->structurally_normalize_();
  if (Exists* inner = dynamic_cast<Exists*>(b.get())) {
    vector<VarDecl> new_decls = this->decls;
    extend(new_decls, inner->decls);
    sort_decls(new_decls);
    return v_exists(new_decls, inner->body);
  } else {
    return v_exists(this->decls, b);
  }
}

value Var::structurally_normalize_() const {
  return v_var(name, sort);
}

value Const::structurally_normalize_() const {
  return v_const(name, sort);
}

value Eq::structurally_normalize_() const {
  return v_eq(left->structurally_normalize_(), right->structurally_normalize_());
}

value Not::structurally_normalize_() const {
  return val->structurally_normalize_()->negate();
}

value Implies::structurally_normalize_() const {
  return v_or({v_not(left), right})->structurally_normalize_();
}

value Apply::structurally_normalize_() const {
  vector<value> new_args;
  for (value const& arg : args) {
    new_args.push_back(arg->structurally_normalize_());
  }
  return v_apply(func->structurally_normalize_(), move(new_args));
}

value structurally_normalize_and_or_or(Value const * the_value) {
  And const * the_and = dynamic_cast<And const*>(the_value);
  Or const * the_or = dynamic_cast<Or const*>(the_value);

  vector<value> args = the_and ? the_and->args : the_or->args;

  vector<vector<VarDecl>> forall_decl_lists;
  vector<vector<VarDecl>> exists_decl_lists;

  while (true) {
    vector<VarDecl> forall_decls;
    vector<VarDecl> exists_decls;
    vector<value> new_args;
    bool found = false;
    for (value arg : args) {
      if (Forall* forall = dynamic_cast<Forall*>(arg.get())) {
        extend(forall_decls, forall->decls);
        arg = forall->body;
        found = true;
      }
      if (Exists* exists = dynamic_cast<Exists*>(arg.get())) {
        extend(exists_decls, exists->decls);
        arg = exists->body;
        found = true;
      }
      new_args.push_back(arg);
    }
    forall_decl_lists.push_back(move(forall_decls));
    exists_decl_lists.push_back(move(exists_decls));
    args = move(new_args);
    if (!found) {
      break;
    }
  }

  vector<value> new_args;
  for (value arg : args) {
    And* sub_and;
    Or* sub_or;
    if (the_and && (sub_and = dynamic_cast<And*>(arg.get()))) {
      extend(new_args, sub_and->args);
    } else if (the_or && (sub_or = dynamic_cast<Or*>(arg.get()))) {
      extend(new_args, sub_or->args);
    } else {
      new_args.push_back(arg);
    }
  }

  value res = the_and ? v_and(new_args) : v_or(new_args);
  for (int i = forall_decl_lists.size() - 1; i >= 0; i--) {
    vector<VarDecl> const& f_decls = forall_decl_lists[i];
    vector<VarDecl> const& e_decls = exists_decl_lists[i];
    if (e_decls.size() > 0) {
      res = v_exists(e_decls, res);
    }
    if (f_decls.size() > 0) {
      res = v_exists(f_decls, res);
    }
  }

  return res;
}

value And::structurally_normalize_() const {
  return structurally_normalize_and_or_or(this);
}

value Or::structurally_normalize_() const {
  return structurally_normalize_and_or_or(this);
}

value TemplateHole::structurally_normalize_() const {
  return v_template_hole();
}

struct ScopeState {
  vector<VarDecl> decls;
};

bool lt_value(value a_, value b_, ScopeState const& ss_a, ScopeState const& ss_b);

int cmp_sort(lsort a, lsort b) {
  if (dynamic_cast<BooleanSort*>(a.get())) {
    if (dynamic_cast<BooleanSort*>(b.get())) {
      return 0;
    }
    else if (UninterpretedSort* usort = dynamic_cast<UninterpretedSort*>(b.get())) {
      return -1;
    }
    else {
      assert(false);
    }
  }
  else if (UninterpretedSort* usort = dynamic_cast<UninterpretedSort*>(b.get())) {
    if (dynamic_cast<BooleanSort*>(a.get())) {
      return 1;
    }
    else if (UninterpretedSort* usort2 = dynamic_cast<UninterpretedSort*>(b.get())) {
      string const& a_name = usort->name;
      string const& b_name = usort2->name;
      if (a_name < b_name) return -1;
      if (a_name > b_name) return 1;
      return 0;
    }
    else {
      assert(false);
    }
  }
  else {
    assert(false);
  }
}

bool eq_sort(lsort a, lsort b) {
  return cmp_sort(a, b) == 0;
}

value forall_exists_normalize_symmetries(
    vector<VarDecl> const& decls,
    set<string> const& vars_used,
    value body,
    bool is_forall,
    int idx,
    ScopeState const& ss)
{
  if (idx == decls.size()) {
    return body->normalize_symmetries(ss, vars_used);
  }

  int idx_end = idx + 1;
  while (idx_end < decls.size() && eq_sort(decls[idx].sort, decls[idx_end].sort)) {
    idx_end++;
  }

  vector<int> perm;
  vector<int> perm_back;
  for (int i = idx; i < idx_end; i++) {
    if (vars_used.find(decls[i].name) == vars_used.end()) {
      perm_back.push_back(i);
    } else {
      perm.push_back(i);
    }
  }
  int perm_front_size = perm.size();
  extend(perm, perm_back);

  value smallest;
  vector<VarDecl> smallest_decls;
  ScopeState ss_smallest;
  do {
    ScopeState ss_ = ss;
    for (int i : perm) {
      ss_.decls.push_back(decls[i]);
    }
    value inner = forall_exists_normalize_symmetries(decls, vars_used, body, is_forall, idx_end, ss_);
    if (smallest == nullptr || lt_value(inner, smallest, ss_, ss_smallest)) {
      smallest = inner;
      ss_smallest = move(ss_);

      smallest_decls.clear();
      for (int i : perm) {
        smallest_decls.push_back(decls[i]);
      }
    }
  } while (next_permutation(perm.begin(), perm.begin() + perm_front_size));

  if (idx_end < decls.size()) {
    if (Forall* inner = dynamic_cast<Forall*>(smallest.get())) {
      smallest = inner->body;
      extend(smallest_decls, inner->decls);
    }
    else if (Exists* inner = dynamic_cast<Exists*>(smallest.get())) {
      smallest = inner->body;
      extend(smallest_decls, inner->decls);
    }
    else {
      assert(false);
    }
  }

  value res = is_forall ? v_forall(smallest_decls, smallest)
                        : v_exists(smallest_decls, smallest);
  return res;
}

value Forall::normalize_symmetries(ScopeState const& ss, set<string> const& vars_used) const {
  return forall_exists_normalize_symmetries(this->decls, vars_used, this->body, true, 0, ss);
}

value Exists::normalize_symmetries(ScopeState const& ss, set<string> const& vars_used) const {
  return forall_exists_normalize_symmetries(this->decls, vars_used, this->body, false, 0, ss);
}

value Var::normalize_symmetries(ScopeState const& ss, set<string> const& vars_used) const {
  return v_var(name, sort);
}

value Const::normalize_symmetries(ScopeState const& ss, set<string> const& vars_used) const {
  return v_const(name, sort);
}

value Eq::normalize_symmetries(ScopeState const& ss, set<string> const& vars_used) const {
  value a = left->normalize_symmetries(ss, vars_used);
  value b = right->normalize_symmetries(ss, vars_used);
  return lt_value(a, b, ss, ss) ? v_eq(a, b) : v_eq(b, a);
}

value Not::normalize_symmetries(ScopeState const& ss, set<string> const& vars_used) const {
  return v_not(val->normalize_symmetries(ss, vars_used));
}

value Implies::normalize_symmetries(ScopeState const& ss, set<string> const& vars_used) const {
  assert(false && "implies should have been replaced by |");
}

value Apply::normalize_symmetries(ScopeState const& ss, set<string> const& vars_used) const {
  vector<value> new_args;
  for (value arg : args) {
    new_args.push_back(arg->normalize_symmetries(ss, vars_used));
  }
  return v_apply(func->normalize_symmetries(ss, vars_used), move(new_args));
}

void sort_values(ScopeState const& ss, vector<value> & values) {
  sort(values.begin(), values.end(), [&ss](value const& a, value const& b) {
    return lt_value(a, b, ss, ss);
  });
}

value And::normalize_symmetries(ScopeState const& ss, set<string> const& vars_used) const {
  vector<value> new_args;
  for (value arg : args) {
    new_args.push_back(arg->normalize_symmetries(ss, vars_used));
  }
  sort_values(ss, new_args);
  return v_and(new_args);
}

value Or::normalize_symmetries(ScopeState const& ss, set<string> const& vars_used) const {
  vector<value> new_args;
  for (value arg : args) {
    new_args.push_back(arg->normalize_symmetries(ss, vars_used));
  }
  sort_values(ss, new_args);
  return v_or(new_args);
}

value TemplateHole::normalize_symmetries(ScopeState const& ss, set<string> const& vars_used) const {
  return v_template_hole();
}

int cmp_expr(value a_, value b_, ScopeState const& ss_a, ScopeState const& ss_b) {
  int a_id = a_->kind_id();
  int b_id = b_->kind_id();
  if (a_id != b_id) return a_id < b_id ? -1 : 1;

  if (Forall* a = dynamic_cast<Forall*>(a_.get())) {
    Forall* b = dynamic_cast<Forall*>(b_.get());
    assert(b != NULL);
  
    if (a->decls.size() < b->decls.size()) return -1;
    if (a->decls.size() > b->decls.size()) return 1;
    ScopeState ss_a_new = ss_a;
    ScopeState ss_b_new = ss_b;
    for (int i = 0; i < a->decls.size(); i++) {
      if (int c = cmp_sort(a->decls[i].sort, b->decls[i].sort)) {
        return c;
      }
      ss_a_new.decls.push_back(a->decls[i]);
      ss_b_new.decls.push_back(b->decls[i]);
    }
    return cmp_expr(a->body, b->body, ss_a_new, ss_b_new);
  }

  if (Exists* a = dynamic_cast<Exists*>(a_.get())) {
    Exists* b = dynamic_cast<Exists*>(b_.get());
    assert(b != NULL);
  
    if (a->decls.size() < b->decls.size()) return -1;
    if (a->decls.size() > b->decls.size()) return 1;
    ScopeState ss_a_new = ss_a;
    ScopeState ss_b_new = ss_b;
    for (int i = 0; i < a->decls.size(); i++) {
      if (int c = cmp_sort(a->decls[i].sort, b->decls[i].sort)) {
        return c;
      }
      ss_a_new.decls.push_back(a->decls[i]);
      ss_b_new.decls.push_back(b->decls[i]);
    }
    return cmp_expr(a->body, b->body, ss_a_new, ss_b_new);
  }

  if (Var* a = dynamic_cast<Var*>(a_.get())) {
    Var* b = dynamic_cast<Var*>(b_.get());
    assert(b != NULL);

    int a_idx = -1, b_idx = -1;
    for (int i = 0; i < ss_a.decls.size(); i++) {
      if (ss_a.decls[i].name == a->name) {
        a_idx = i;
        break;
      }
    }
    for (int i = 0; i < ss_b.decls.size(); i++) {
      if (ss_b.decls[i].name == b->name) {
        b_idx = i;
        break;
      }
    }
    assert (a_idx != -1);
    assert (b_idx != -1);
    return a_idx < b_idx ? -1 : (a_idx == b_idx ? 0 : 1);
  }

  if (Const* a = dynamic_cast<Const*>(a_.get())) {
    Const* b = dynamic_cast<Const*>(b_.get());
    assert(b != NULL);

    return a->name < b->name ? -1 : (a->name == b->name ? 0 : 1);
  }

  if (Eq* a = dynamic_cast<Eq*>(a_.get())) {
    Eq* b = dynamic_cast<Eq*>(b_.get());
    assert(b != NULL);

    if (int c = cmp_expr(a->left, b->left, ss_a, ss_b)) return c;
    return cmp_expr(a->right, b->right, ss_a, ss_b);
  }

  if (Not* a = dynamic_cast<Not*>(a_.get())) {
    Not* b = dynamic_cast<Not*>(b_.get());
    assert(b != NULL);

    return cmp_expr(a->val, b->val, ss_a, ss_b);
  }

  if (Implies* a = dynamic_cast<Implies*>(a_.get())) {
    Implies* b = dynamic_cast<Implies*>(b_.get());
    assert(b != NULL);

    if (int c = cmp_expr(a->left, a->right, ss_a, ss_b)) return c;
    return cmp_expr(a->left, a->right, ss_a, ss_b);
  }

  if (Apply* a = dynamic_cast<Apply*>(a_.get())) {
    Apply* b = dynamic_cast<Apply*>(b_.get());
    assert(b != NULL);

    if (int c = cmp_expr(a->func, b->func, ss_a, ss_b)) return c;

    if (a->args.size() < b->args.size()) return -1;
    if (a->args.size() > b->args.size()) return 1;

    for (int i = 0; i < a->args.size(); i++) {
      if (int c = cmp_expr(a->args[i], b->args[i], ss_a, ss_b)) {
        return c;
      }
    }

    return 0;
  }

  if (And* a = dynamic_cast<And*>(a_.get())) {
    And* b = dynamic_cast<And*>(b_.get());
    assert(b != NULL);

    if (a->args.size() < b->args.size()) return -1;
    if (a->args.size() > b->args.size()) return 1;

    for (int i = 0; i < a->args.size(); i++) {
      if (int c = cmp_expr(a->args[i], b->args[i], ss_a, ss_b)) {
        return c;
      }
    }

    return 0;
  }

  if (Or* a = dynamic_cast<Or*>(a_.get())) {
    Or* b = dynamic_cast<Or*>(b_.get());
    assert(b != NULL);

    if (a->args.size() < b->args.size()) return -1;
    if (a->args.size() > b->args.size()) return 1;

    for (int i = 0; i < a->args.size(); i++) {
      if (int c = cmp_expr(a->args[i], b->args[i], ss_a, ss_b)) {
        return c;
      }
    }

    return 0;
  }

  assert(false);
}

bool lt_value(value a_, value b_, ScopeState const& ss_a, ScopeState const& ss_b) {
  return cmp_expr(a_, b_, ss_a, ss_b) < 0;
}

value Forall::indexify_vars(map<string, string> const& m) const {
  map<string, string> new_m = m;
  vector<VarDecl> new_decls;
  for (VarDecl const& decl : this->decls) {
    string new_name = "A." + ::to_string(new_m.size() + 1);
    new_m[decl.name] = new_name;
    new_decls.push_back(VarDecl(new_name, decl.sort));
  }
  return v_forall(new_decls, body->indexify_vars(new_m));
}

value Exists::indexify_vars(map<string, string> const& m) const {
  map<string, string> new_m = m;
  vector<VarDecl> new_decls;
  for (VarDecl const& decl : this->decls) {
    string new_name = "A." + ::to_string(new_m.size() + 1);
    new_m[decl.name] = new_name;
    new_decls.push_back(VarDecl(new_name, decl.sort));
  }
  return v_forall(new_decls, body->indexify_vars(new_m));
}

value Var::indexify_vars(map<string, string> const& m) const {
  auto iter = m.find(this->name);
  assert(iter != m.end());
  return v_var(iter->second, this->sort);
}

value Const::indexify_vars(map<string, string> const& m) const {
  return v_const(name, sort);
}

value Eq::indexify_vars(map<string, string> const& m) const {
  return v_eq(left->indexify_vars(m), right->indexify_vars(m));
}

value Not::indexify_vars(map<string, string> const& m) const {
  return v_not(this->val->indexify_vars(m));
}

value Implies::indexify_vars(map<string, string> const& m) const {
  return v_implies(left->indexify_vars(m), right->indexify_vars(m));
}

value Apply::indexify_vars(map<string, string> const& m) const {
  vector<value> new_args;
  for (value const& arg : args) {
    new_args.push_back(arg->indexify_vars(m));
  }
  return v_apply(func->indexify_vars(m), move(new_args));
}

value And::indexify_vars(map<string, string> const& m) const {
  vector<value> new_args;
  for (value const& arg : args) {
    new_args.push_back(arg->indexify_vars(m));
  }
  return v_and(move(new_args));
}

value Or::indexify_vars(map<string, string> const& m) const {
  vector<value> new_args;
  for (value const& arg : args) {
    new_args.push_back(arg->indexify_vars(m));
  }
  return v_or(move(new_args));
}

value TemplateHole::indexify_vars(map<string, string> const& m) const {
  return v_template_hole();
}

void Forall::get_used_vars(set<string>& s) const {
  body->get_used_vars(s);
}
void Exists::get_used_vars(set<string>& s) const {
  body->get_used_vars(s);
}
void Var::get_used_vars(set<string>& s) const {
  s.insert(name);
}
void Const::get_used_vars(set<string>& s) const {
}
void Eq::get_used_vars(set<string>& s) const {
  left->get_used_vars(s);
  right->get_used_vars(s);
}
void Not::get_used_vars(set<string>& s) const {
  val->get_used_vars(s);
}
void Implies::get_used_vars(set<string>& s) const {
  left->get_used_vars(s);
  right->get_used_vars(s);
}
void Apply::get_used_vars(set<string>& s) const {
  func->get_used_vars(s);
  for (value arg : args) {
    arg->get_used_vars(s);
  }
}
void And::get_used_vars(set<string>& s) const {
  for (value arg : args) {
    arg->get_used_vars(s);
  }
}
void Or::get_used_vars(set<string>& s) const {
  for (value arg : args) {
    arg->get_used_vars(s);
  }
}
void TemplateHole::get_used_vars(set<string>& s) const {
}

int counter = 0;
Benchmarking bench;

value Value::totally_normalize() const {

  ScopeState ss;

  bench.start("structurally_normalize");
  value res = this->structurally_normalize();
  bench.end();

  set<string> vars_used;
  res->get_used_vars(vars_used);

  bench.start("normalize_symmetries");
  res = res->normalize_symmetries(ss, vars_used);
  bench.end();

  bench.start("indexify_vars");
  res = res->indexify_vars({});
  bench.end();

  counter++;
  if (counter % 1000 == 0) {
    printf("count = %d\n", counter);
    bench.dump();
  }

  return res;
}
