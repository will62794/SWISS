#ifndef VAR_LEX_GRAPH_H
#define VAR_LEX_GRAPH_H

#include "logic.h"

struct VarIndexState {
  // Minimum index reached (0 for none at all, 1 for having reached
  // the first one, etc.)
  std::vector<int> indices;
};

struct VarIndexTransitionPrecondition {
  // Largest number which needs to have been reached before going in.
  // (e.g., if the term has 1 2 5, then the index stored here would be 4.)
  std::vector<int> indices;
};

struct VarIndexTransitionResult {
  // Minimum index reached (0 for none at all, 1 for having reached
  // the first one, etc.)
  std::vector<int> indices;
};

inline void var_index_do_transition(
  VarIndexState const& state,
  VarIndexTransitionResult const& trans,
  VarIndexState& result)
{
  for (int i = 0; i < state.indices.size(); i++) {
    result.indices[i] = state.indices[i] > trans.indices[i] ?
        state.indices[i] : trans.indices[i];
  }
}

inline bool var_index_is_valid_transition(
    VarIndexState const& state,
    VarIndexTransitionPrecondition const& pre)
{
  for (int i = 0; i < state.indices.size(); i++) {
    if (pre.indices[i] > state.indices[i]) {
      return false;
    }
  }
  return true;
}

struct VarIndexTransition {
  VarIndexTransitionPrecondition pre;
  VarIndexTransitionResult res;
};

std::vector<VarIndexTransition> get_var_index_transitions(
  value templ,
  std::vector<value> const& values);

VarIndexState get_var_index_init_state(
  value templ);

#endif
