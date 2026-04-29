#pragma once

#include "clauses.hpp"
#include "assignment.hpp"

#include <optional>
#include <queue>

namespace peregrine::propagate {

	using namespace peregrine::core;
	using namespace peregrine::clauses;

	template<typename Prop, typename Clause>
	concept Propagator = ClauseStorage<Clause> && requires(Prop prop, const Clause & clauses, Assignment & assignment) {
		{ prop.propagate(assignment) } -> std::same_as<std::optional<size_t>>; // returns conflicting clause's ID
	};

	template<ClauseStorage Clauses>
	class SimpleBCP {
	private:
		size_t curr_base_level = -1;

		// helper struct to index watches easily
		struct Watches {
		private:
			inline size_t literal_index(Lit x) const noexcept {
				return static_cast<size_t>(2) * LitToVar(x).val + ifNeg(x);
			}

		public:

			explicit Watches(size_t numVars) : _watches(2 * numVars + 1) { }

			std::vector<std::vector<size_t>> _watches;

			std::vector<size_t>& operator[](Lit lit) {
				return _watches[literal_index(lit)];
			}

			const std::vector<size_t>& operator[](Lit lit) const {
				return _watches[literal_index(lit)];
			}
		};

		Clauses& clauses;
		Watches watches;

	public:
		// Expects an empty assignment but filled clauses
		explicit SimpleBCP(Clauses& _clauses) 
			: clauses(_clauses)
			, watches(clauses.num_vars())
		{
			for (size_t clause_idx : std::views::iota(size_t{ 0 }, clauses.num_clauses())) {
				auto& curr_clause = clauses.get_clause(clause_idx).literals;

				for (auto watched_lit : curr_clause | std::views::take(2)) {
					watches[watched_lit].push_back(clause_idx);
				}
			}
		}

		auto propagate(Assignment& assignment) -> std::optional<size_t>;
	};

} // namespace peregrine::propagate