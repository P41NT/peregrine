#pragma once

#include <vector>
#include "types.hpp"
#include <assert.h>

namespace peregrine::clauses {

	using namespace peregrine::core;

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
			: assignments(numLiterals, LBool::UNDEF) {
		}

		LBool getVar(Var x) const noexcept;
		LBool getLit(Lit x) const noexcept;

		void new_level() noexcept;

		void enqueue(Var x, LBool value, size_t reason_idx) noexcept;
		void enqueue(Lit x, size_t reason_idx) noexcept;

		void pop_to_level(size_t level) noexcept;

		size_t getCurrentLevel() const noexcept;

		Lit getLitAtLevel(size_t level) const noexcept;
	};
}