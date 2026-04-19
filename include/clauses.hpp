#pragma once

#include <concepts>
#include "assignment.hpp"
#include <ranges>
#include <algorithm>

namespace peregrine::clauses {

	using namespace peregrine::core;

	template <typename T>
	concept ClauseStorage = requires(T c, Assignment &assignment, std::vector<Lit>& clause_lits, size_t numVariables, Lit lit, size_t clause_id) {
		T(numVariables);
		{ c.is_satisfied(assignment) } -> std::convertible_to<bool>;
		{ c.add_clause(std::move(clause_lits)) };
		{ c.add_clause(clause_lits) };
		{ c.num_clauses() } -> std::convertible_to<size_t>;
		{ c.num_vars() } -> std::convertible_to<size_t>;
		{ c.watch_list(lit) } -> std::same_as<std::vector<size_t>&>;
		{ c.get_clause(clause_id) } -> std::same_as<std::span<const Lit>>;
	};

	class ClauseStoreDefault {
		// Default in CNF form ig.
		struct CNFClause {
			std::vector<Lit> literals;
			bool is_satisfied(const Assignment& assignment) const {
				return std::any_of(literals.begin(), literals.end(), [&assignment](auto x) {
					return assignment.getLit(x) == LBool::TRUE;
				});
			}

			explicit CNFClause(std::vector<Lit>&& lits)
				: literals(std::move(lits))
			{ }
		};

		size_t numVariables;
		std::vector<CNFClause> clauses;
		std::vector<std::vector<size_t>> watches;

	public:
		explicit ClauseStoreDefault(size_t numVariables)
			: numVariables(numVariables),
			watches(2 * (numVariables + 1))
		{ }

		bool is_satisfied(const Assignment& assignment) const noexcept {
			return std::all_of(clauses.begin(), clauses.end(), [&assignment](auto& c) {
				return c.is_satisfied(assignment);
			});
		}

		size_t num_clauses() const noexcept {
			return clauses.size();
		}

		size_t num_vars() const noexcept {
			return numVariables;
		}

		template <typename T> requires std::convertible_to<T, std::vector<Lit>>
		void add_clause(T&& clause) {
			clauses.emplace_back(std::forward<T>(clause));
		}

		auto watch_list(Lit lit) -> std::vector<size_t>& {
			return watches[literal_index(lit)];
		}

		auto get_clause(size_t clause_id) const -> std::span<const Lit> {
			return clauses[clause_id].literals;
		}

	private:
		inline size_t literal_index(Lit x) const noexcept {
			return static_cast<size_t>(2) * LitToVar(x) + ifNeg(x);
		}
	};
}
