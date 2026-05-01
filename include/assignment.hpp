#pragma once

#include "types.hpp"

#include <assert.h>
#include <ranges>
#include <vector>
#include <optional>

namespace peregrine::clauses {

using namespace peregrine::core;

class Assignment {
private:
  std::vector<LBool> assignments;
  std::vector<Lit> trails;
  std::vector<size_t> trail_lim;
  std::vector<size_t> cause_idxs;

  size_t numVariables = 0;
  size_t currentLevel = 0;

public:
  explicit Assignment(size_t numLiterals)
      : assignments(numLiterals + 1, LBool::UNDEF), numVariables(numLiterals) {
    // reserve enough space for all literals to be assigned at the same level
    // HUGE EDGE CASE: if we don't reserve enough space, then when we pop to
    // level 0, the std::span returned by getLitsAtLevel(0) will be invalidated
    // and cause undefined behavior when accessed
    trails.reserve(numLiterals + 1);
    trail_lim.reserve(numLiterals + 1);
    cause_idxs.reserve(numLiterals + 1);
  }

  LBool getVar(Var x) const noexcept;
  LBool getLit(Lit x) const noexcept;

  void new_level() noexcept;

  void enqueue(Var x, LBool value, size_t reason_idx) noexcept;
  void enqueue(Lit x, size_t reason_idx) noexcept;

  void pop_to_level(size_t level) noexcept;

  size_t getCurrentLevel() const noexcept;

  std::span<const Lit> getLitsAtLevel(size_t level) const noexcept;

  auto vars() const -> decltype(auto) {
    return std::views::iota(size_t{1}, numVariables + 1) |
           std::views::transform([](int i) { return Var(i); });
  }

  auto getAllAssignments() -> std::optional<std::vector<Lit>>;
};
} // namespace peregrine::clauses