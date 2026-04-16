#pragma once

#include <concepts>
#include "assignment.h"
#include <ranges>
#include <algorithm>

namespace peregrine {

	template <typename T>
	concept ClauseStorage = requires(T c, Assignment &assignment, std::vector<int> clause_lits) {
		{ c.is_satisfied(assignment) } -> std::convertible_to<bool>;
		{ c.add_clause(std::move(clause_lits)) };
	};

	class ClauseStoreDefault {
		// Default in CNF form ig.
		struct CNFClause {
			std::vector<Lit> literals;
			bool is_satisfied(const Assignment& assignment) {
				return std::any_of(literals.begin(), literals.end(), [&assignment](auto x) {
					return assignment.getLit(x);
				});
			}

			explicit CNFClause(std::vector<Lit>& lits)
				: literals(std::move(lits))
			{ }
		};

		size_t numVariables;
		std::vector<CNFClause> clauses;

	public:
		explicit ClauseStoreDefault(size_t numVariables)
			: numVariables(numVariables)
		{ }

		bool is_satisfied(const Assignment& assignment) noexcept {
			return std::all_of(clauses.begin(), clauses.end(), [&assignment](auto& c) {
				return c.is_satisfied(assignment);
			});
		}
	};
}
