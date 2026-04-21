#pragma once

namespace peregrine::core {

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

	inline bool isUndefined(LBool x) {
		return x == LBool::UNDEF;
	}

	// for strong-typing between Var, Lit and other integers
	template<typename Tag>
	struct StrongInt {
		explicit StrongInt(int v) : val(v) {}
		int val;
	};

	struct LitTag {};
	struct VarTag {};

	using Lit = StrongInt<LitTag>;
	using Var = StrongInt<VarTag>;

	// explicit conversions between Var and Lit
	inline Var LitToVar(Lit x) {
		int val = x.val;
		return static_cast<Var>(val < 0 ? -val : val);
	}

	inline Lit VarToLit(Var x, bool set = true) {
		int val = x.val;
		return set ? Lit{ val } : Lit{ -val };
	}

	inline bool ifNeg(Lit x) {
		return x.val < 0;
	}
}
