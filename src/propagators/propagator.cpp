#include "../../include/propagator.hpp"

namespace peregrine::propagate {

	template<ClauseStorage Clauses>
	auto SimpleBCP<Clauses>::propagate(Clauses& clauses, Assignment& assignment) -> std::optional<size_t> {
	}

} 