#pragma once

#include "clauses.h"

namespace peregrine {
	template <ClauseStorage CStore>
	class DIMACSCNFParser {
		CStore& clause_store;
	public:
		explicit DIMACSCNFParser(CStore& store) : clause_store(store) {}
		void parse(std::istream& input_stream)
	};
}
