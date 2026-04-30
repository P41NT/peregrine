#pragma once

#include "assignment.hpp"

#include <algorithm>
#include <concepts>
#include <ranges>

namespace peregrine::clauses {
using namespace peregrine::core;

// Default in CNF form ig.
struct CNFClause {
  std::vector<Lit> literals;
  bool is_satisfied(const Assignment &assignment) const {
    return std::any_of(literals.begin(), literals.end(), [&assignment](auto x) {
      return assignment.getLit(x) == LBool::TRUE;
    });
  }

  bool not_satisfied(const Assignment &assignment) const {
    return std::all_of(literals.begin(), literals.end(), [&assignment](auto x) {
      return assignment.getLit(x) == LBool::FALSE;
    });
  }

  template <typename T>
    requires std::constructible_from<std::vector<Lit>, T>
  explicit CNFClause(T &&lits) : literals(std::forward<T>(lits)) {}
};

template <typename T>
concept ClauseStorage =
    requires(T c, Assignment &assignment, std::vector<Lit> &clause_lits,
             size_t numVariables, Lit lit, size_t clause_id) {
      { c.is_satisfied(assignment) } -> std::convertible_to<bool>;
      { c.not_satisfied(assignment) } -> std::convertible_to<bool>;
      { c.add_clause(std::move(clause_lits)) };
      { c.add_clause(clause_lits) };
      { c.num_clauses() } -> std::convertible_to<size_t>;
      { c.num_vars() } -> std::convertible_to<size_t>;
      { c.get_clause(clause_id) } -> std::same_as<CNFClause &>;
    };

class ClauseStoreDefault {
  size_t numVariables;
  std::vector<CNFClause> clauses;

public:
  explicit ClauseStoreDefault(size_t numVariables)
      : numVariables(numVariables) {}

  bool is_satisfied(const Assignment &assignment) const noexcept {
    return std::all_of(clauses.begin(), clauses.end(), [&assignment](auto &c) {
      return c.is_satisfied(assignment);
    });
  }

  bool not_satisfied(const Assignment &assignment) const noexcept {
    return std::any_of(clauses.begin(), clauses.end(), [&assignment](auto &c) {
      return c.not_satisfied(assignment);
    });
  }

  size_t num_clauses() const noexcept { return clauses.size(); }

  size_t num_vars() const noexcept { return numVariables; }

  template <typename T>
    requires std::constructible_from<std::vector<Lit>, T>
  void add_clause(T &&clause) {
    clauses.emplace_back(std::forward<T>(clause));
  }

  CNFClause &get_clause(size_t clause_id) noexcept {
    auto &req_clause = clauses[clause_id];
    return req_clause;
  }

private:
  inline size_t literal_index(Lit x) const noexcept {
    return static_cast<size_t>(2) * LitToVar(x).val + ifNeg(x);
  }
};

static_assert(ClauseStorage<ClauseStoreDefault>);

template <typename T>
concept SharedClausePool =
    ClauseStorage<T> && requires(T pool, CNFClause clause) {
      { pool.export_clause(clause) } -> std::same_as<void>;
      { pool.import_clauses() } -> std::same_as<std::vector<CNFClause>>;
    };

// Default placeholder for SharedClausePool when there is no sharing.
struct NoSharing : ClauseStoreDefault {
  // temporarily inherting this instead of making dummy vars
  explicit NoSharing() : ClauseStoreDefault(0) {}

  void export_clause(CNFClause) {}
  std::vector<CNFClause> import_clauses() { return {}; };
};

static_assert(SharedClausePool<NoSharing>);

} // namespace peregrine::clauses
