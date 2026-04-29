#pragma once

namespace peregrine::heuristic {
	template<typename H>
	concept DecisionHeuristic = requires(H h, Var v, LBool value, const Assignment & assignment) {
		{ h.choose_var(assignment) } -> std::same_as<Var>;
		{ h.on_assign(v, value); } -> std::same_as<void>;
		{ h.on_unassign(v); } -> std::same_as<void>;
		{ h.on_conflict(v); } -> std::same_as<void>;
	}
}
