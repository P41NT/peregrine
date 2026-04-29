#pragma once

#include "clauses.hpp"
#include "assignment.hpp"
#include "propagator.hpp"
#include "parser.hpp"

namespace peregrine::solver::dpll {
	using namespace peregrine::core;
	using namespace peregrine::clauses;
	using namespace peregrine::propagate;
	using namespace peregrine::parse;


	template<typename T, typename Clauses, typename Prop, typename SharedPool>
	concept DPLLSolver =
		ClauseStorage<Clauses> &&
		Propagator<Prop, Clauses> &&
		SharedClausePool<SharedPool> &&
	requires(T d, Clauses& clauses, Assignment& assignment, std::string filename) {
		T(clauses);
		{ d.solve() } -> std::same_as<std::optional<std::vector<LBool>>>;
	};

	template<typename Clauses, typename Prop, typename SharedPool = NoSharing> 
	requires ClauseStorage<Clauses> && Propagator<Prop, Clauses>
	class DPLLSolverDefault {
	private:
		Clauses clause_db;
		Assignment assignment;
		Prop propagator;

	public:
		explicit DPLLSolverDefault(Clauses clauses) 
			: clause_db(std::move(clauses))
			, assignment(clause_db.num_vars)
			, propagator(clause_db)
		{ }

		explicit DPLLSolverDefault(std::string&& filename)
			: clause_db(dimacs_cnf_parse<ClauseStorage>(std::ifstream(filename)))
			, assignment(clause_db.num_vars()),
			, propagator(clause_db)
		{ }

		auto solve() noexcept -> std::optional<std::vector<LBool>>;
	};

	template DPLLSolverDefault<ClauseStoreDefault, SimpleBCP<ClauseStoreDefault>, NoSharing>;
};
