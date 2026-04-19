#include "../../include/assignment.hpp"


namespace peregrine::clauses {

	LBool Assignment::getVar(Var x) const noexcept { return assignments[x]; }
	LBool Assignment::getLit(Lit x) const noexcept { return ifNeg(x) ? !assignments[LitToVar(x)] : assignments[LitToVar(x)]; }

	void Assignment::new_level() noexcept {
		trail_lim.push_back(assignments.size());
		currentLevel++;
	}

	void Assignment::enqueue(Var x, LBool value, size_t reason_idx) noexcept {
		assert(trails.size() == cause_idxs.size());
		assert(value != LBool::UNDEF);

		Lit trail_lit = VarToLit(x, value == LBool::TRUE);

		trails.emplace_back(trail_lit);
		cause_idxs.emplace_back(reason_idx);
		assignments[x] = value;
	}

	void Assignment::pop_to_level(size_t level) noexcept {
		assert(level < getCurrentLevel());

		currentLevel = level;
		size_t trail_size = trail_lim[level];

		while (trails.size() > trail_size) {
			auto x = trails.back();
			assignments[x] = LBool::UNDEF;
			trails.pop_back();
		}

		trail_lim.resize(level);
		cause_idxs.resize(trail_size);
	}

	size_t Assignment::getCurrentLevel() const noexcept { return currentLevel; }
}