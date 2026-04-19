#pragma once

#include "clauses.hpp"

namespace peregrine::parse {

	using namespace peregrine::core;
	using namespace peregrine::clauses;

	template <ClauseStorage CStore>
	auto dimacs_cnf_parse(std::istream& input_stream) -> CStore;

}
