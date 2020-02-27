#include "filter.h"
#include "contexts.h"

using namespace std;

bool is_necessary(
    smt::context& ctx,
    shared_ptr<Module> module,
    vector<value> values,
    int i)
{
  BasicContext basic_ctx(ctx, module);
  for (int j = 0; j < (int)values.size(); j++) {
    if (j == i) {
      basic_ctx.e->value2expr(v_not(values[i]));
    } else {
      basic_ctx.e->value2expr(values[i]);
    }
  }

  smt::solver& solver = basic_ctx.ctx->solver;
  return solver.check_sat();
}

vector<value> filter_redundant_formulas(
  shared_ptr<Module> module,
  vector<value> values)
{
  smt::context ctx;

  for (int i = 0; i < (int)values.size(); i++) {
    if (!is_necessary(ctx, module, values, i)) {
      for (int j = i; j < (int)values.size() - 1; j++) {
        values[j] = values[j+1];
      }
      values.pop_back();
      i--;
    }
  }

  return values;
}
