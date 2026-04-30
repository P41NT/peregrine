#include "../../include/assignment.hpp"


namespace peregrine::clauses {

	LBool Assignment::getVar(Var x) const noexcept { return assignments[x.val]; }
	LBool Assignment::getLit(Lit x) const noexcept { return ifNeg(x) ? !assignments[LitToVar(x).val] : assignments[LitToVar(x).val]; }

	void Assignment::new_level() noexcept {
		trail_lim.push_back(trails.size());
		currentLevel++;
	}

	void Assignment::enqueue(Var x, LBool value, size_t reason_idx) noexcept {
		assert(trails.size() == cause_idxs.size());
		assert(value != LBool::UNDEF);
		assert(x.val < assignments.size());

		Lit trail_lit = VarToLit(x, value == LBool::TRUE);

		trails.emplace_back(trail_lit);
		cause_idxs.emplace_back(reason_idx);
		assignments[x.val] = value;
	}

	void Assignment::enqueue(Lit x, size_t reason_idx) noexcept {
		Var x_var = LitToVar(x);
		LBool x_val = getSign(x);

		enqueue(x_var, x_val, reason_idx);
	}

	void Assignment::pop_to_level(size_t level) noexcept {
		assert(level < getCurrentLevel());

		currentLevel = level;
		size_t trail_size = trail_lim[level];

		while (trails.size() > trail_size) {
			Var x = LitToVar(trails.back());
			assignments[x.val] = LBool::UNDEF;
			trails.pop_back();
		}

		trail_lim.resize(level);
		cause_idxs.resize(trail_size);
	}

	size_t Assignment::getCurrentLevel() const noexcept { return currentLevel; }

	std::span<Lit> Assignment::getLitsAtLevel(size_t level) noexcept {
		assert(level >= 0 && level < trails.size());
		auto trail_start = level == 0 ? 0 : trail_lim[level - 1];
		auto trail_end = trail_lim[level];

		return std::span<Lit>(trails.data() + trail_start, trail_end - trail_start);
	}

}