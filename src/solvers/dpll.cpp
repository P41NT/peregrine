#include "../../include/dpll.hpp"

namespace peregrine::solver::dpll {
	using namespace peregrine::core;
	using namespace peregrine::clauses;
	using namespace peregrine::propagate;

	template<typename Clauses, typename Prop, typename SharedPool>
	auto DPLLSolverDefault<Clauses, Prop, SharedPool>::solve() -> std::optional<std::vector<LBool>> {
		if (!propagator.propagate(assignment)) {
			return std::nullopt; // unsatisfiable
		}
	}
}
