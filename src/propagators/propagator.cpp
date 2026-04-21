#include "../../include/propagator.hpp"

#include <ranges>

namespace peregrine::propagate {

	using namespace peregrine::core;
	using namespace peregrine::clauses;

	template<ClauseStorage Clauses>
	auto SimpleBCP<Clauses>::propagate(Assignment& assignment) -> std::optional<size_t> {
		for (; curr_base_level < assignment.getCurrentLevel(); curr_base_level++) {

			Lit p = assignment.getLitAtLevel(curr_base_level);
			Lit np = !p;

			auto& curr_watch = watches[np];

			std::optional<size_t> conflict_idx = {};

			std::erase_if(curr_watch, [&assignment, &watches, &clauses](size_t clause_idx) {
				auto curr_clause = clauses.get_clause(clause_idx);

				size_t false_idx = (curr_clause[0] == np ? 0 : 1);
				size_t other_idx = false_idx ^ 1;
				
				if (assignment.getLit(curr_clause[other_idx]) == LBool::TRUE) { // satisfied
					watches[other_idx].push_back(clause_idx);
					return true; // stays in watch[np]
				}

				for (auto const [lit, lit_idx] : curr_clause | std::views::drop(2) | std::views::enumerate) {
					if (assignment.getLit(lit) != LBool::FALSE) {
						watches[lit_idx].push_back(clause_idx);
						std::swap(curr_clause[false_idx], curr_clause[lit_idx]);
						return false; // removed from watch[np]
					}
				}

				if (assignment.getLit(curr_clause[other_idx]) == LBool::UNDEF) {
					assignment.enqueue(curr_clause[other_idx], clause_idx);
					return true; // stays in watch[np]
				}
				else {
					conflict_idx.value = clause_idx;
					return true; // stays in watch[np]
				}
			});
		}
	}
} 