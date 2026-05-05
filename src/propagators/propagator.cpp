#include "../../include/propagator.hpp"

#include <iostream>
#include <ranges>

namespace peregrine::propagate {

using namespace peregrine::core;
using namespace peregrine::clauses;

template <ClauseStorage Clauses>
auto SimpleBCP<Clauses>::propagate_lit(Lit literal, Assignment &assignment)
    -> std::optional<size_t> {
  const Lit np = ~literal;
  auto &curr_watch = watches[np];

  std::optional<size_t> conflict_idx = std::nullopt;

  std::erase_if(curr_watch, [=, &assignment, &conflict_idx](size_t clause_idx) {
    std::vector<Lit> &curr_clause = clauses.get_clause(clause_idx).literals;

    // assuming curr_clause has size at least 2, initial propagate must cover
    // these cases
    assert(curr_clause.size() >= 2);
    // we get the index of np (it is guaranteed to be in the first 2)
    assert(curr_clause[0] == np || curr_clause[1] == np);

    size_t false_idx = (curr_clause[0] == np ? 0 : 1);
    size_t other_idx = false_idx ^ 1;

    // if other_idx is satisfied, this clause is done
    if (assignment.getLit(curr_clause[other_idx]) == LBool::TRUE) { // satisfied
      return false; // stays in watch[np]
    }

    for (auto const [lit_idx, lit] :
         curr_clause | std::views::enumerate | std::views::drop(2)) {
      if (assignment.getLit(lit) != LBool::FALSE) {
        watches[lit].push_back(clause_idx);
        std::swap(curr_clause[false_idx], curr_clause[lit_idx]);
        return true; // removed from watch[np]
      }
    }

    if (assignment.getLit(curr_clause[other_idx]) == LBool::UNDEF) {
      assignment.enqueue(curr_clause[other_idx], clause_idx);
      prop_queue.push_back(curr_clause[other_idx]);
      return false; // stays in watch[np]
    } else {
      conflict_idx.emplace(clause_idx);
      return true; // stays in watch[np]
    }
  });

  return conflict_idx;
}

template <ClauseStorage Clauses>
auto SimpleBCP<Clauses>::propagate(Assignment &assignment)
    -> std::optional<size_t> {

  std::optional<size_t> clause_idx = std::nullopt;
  for (; curr_base_level <= assignment.getCurrentLevel(); curr_base_level++) {
    auto level_lits = assignment.getLitsAtLevel(curr_base_level);
    for (Lit l : level_lits) {
      prop_queue.push_back(l);
    }
  }

  while (!prop_queue.empty()) {
    Lit curr_lit = prop_queue.back();
    prop_queue.pop_back();

    auto conflict = propagate_lit(curr_lit, assignment);
    if (conflict.has_value())
      return conflict;
  }
  return std::nullopt;
}

template class SimpleBCP<ClauseStoreDefault>;
} // namespace peregrine::propagate