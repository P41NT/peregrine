#pragma once

namespace peregrine {

	enum class LBool {
		TRUE,
		FALSE,
		UNDEF
	};

	inline LBool operator!(LBool x) {
		switch (x) {
		case LBool::TRUE:
			return LBool::FALSE;
		case LBool::FALSE:
			return LBool::TRUE;
		case LBool::UNDEF:
			return LBool::UNDEF;
		}
	}

	bool isUndefined(LBool x) {
		return x == LBool::UNDEF;
	}

	using Var = uint8_t;
	using Lit = int8_t;

	inline Var LitToVar(Lit x) {
		return (x < 0 ? -x : x);
	}

	inline Lit VarToLit(Var x, bool set) {
		return set ? x : -x;
	}

	bool ifNeg(Lit x) {
		return x < 0;
	}
}
