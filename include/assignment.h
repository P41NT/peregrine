#pragma once

#include <vector>
#include <memory_resource>
#include "types.h"
#include <assert.h>

namespace peregrine {

	class Assignment {
	private:
		std::vector<LBool> assignments;
		std::vector<Lit> trails;
		std::vector<size_t> trail_lim;
		std::vector<size_t> cause_idxs;

		size_t numVariables = 0;
		size_t currentLevel = 0;

	public:
		explicit Assignment(size_t numLiterals)
			: assignments(numLiterals) {
		}

		LBool getVar(Var x) const noexcept { return assignments[x]; }
		LBool getLit(Lit x) const noexcept { return ifNeg(x) ? !assignments[LitToVar(x)] : assignments[LitToVar(x)]; }

		void new_level() noexcept {
			trail_lim.push_back(assignments.size());
			currentLevel++;
		}

		void enqueue(Var x, LBool value, size_t reason_idx) noexcept {
			assert(trails.size() == cause_idxs.size());
			assert(value != LBool::UNDEF);

			Lit trail_lit = VarToLit(x, value == LBool::TRUE);

			trails.emplace_back(trail_lit);
			cause_idxs.emplace_back(reason_idx);
			assignments[x] = value;
		}

		void pop_to_level(size_t level) noexcept {
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

		size_t getCurrentLevel() const noexcept { return currentLevel; }
	};
}