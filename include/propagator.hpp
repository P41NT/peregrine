#pragma once

#include "clauses.hpp"
#include "assignment.hpp"

#include <optional>

namespace peregrine::propagate {

	using namespace peregrine::core;
	using namespace peregrine::clauses;

	template<typename Prop, typename Clause> 
	concept Propagator = ClauseStorage<Clause> && requires(Prop prop, const Clause& clauses, Assignment& assignment) 
	{
		{ prop.propagate(clauses, assignment) } -> std::same_as<std::optional<size_t>>; // returns conflicting clause's ID
	};

	template<ClauseStorage Clauses>
	class SimpleBCP {
		auto propagate(Clauses& clauses, Assignment& assignment) -> std::optional<size_t>;
	};

} // namespace peregrine::propagate