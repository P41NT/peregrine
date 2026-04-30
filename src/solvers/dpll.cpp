#include "../../include/dpll.hpp"

namespace peregrine::solver::dpll {
	using namespace peregrine::core;
	using namespace peregrine::clauses;
	using namespace peregrine::propagate;

	template<typename Clauses, typename Prop, typename SharedPool> 
	requires ClauseStorage<Clauses> && Propagator<Prop, Clauses>
	auto DPLLSolverDefault<Clauses, Prop, SharedPool>::solve() noexcept -> std::optional<std::vector<LBool>> {
		if (!propagator.propagate(assignment)) {
			return std::nullopt; // unsatisfiable
		}

		for (Var v : assignment.vars()) {
			if (assignment.getVar(v) == LBool::UNDEF) {

				assignment.new_level();
				assignment.enqueue(v, LBool::TRUE, 0);

				// search for true assignment
				if (auto res = solve()) {
					return res;
				}
				
				// no solve
				assignment.pop_to_level(assignment.getCurrentLevel() - 1);
				assignment.new_level();
				assignment.enqueue(v, LBool::FALSE, 0);

				// search for false assignment
				if (auto res = solve()) {
					return res;
				}

				assignment.pop_to_level(assignment.getCurrentLevel() - 1);
				return std::nullopt; // unsatisfiable
			}
		}
	}

	template class DPLLSolverDefault<ClauseStoreDefault, SimpleBCP<ClauseStoreDefault>>;
}
