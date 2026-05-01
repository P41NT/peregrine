#pragma once

#include "clauses.hpp"

namespace peregrine::parse {

using namespace peregrine::core;
using namespace peregrine::clauses;

template <ClauseStorage CStore>
auto dimacs_cnf_parse(std::istream &input_stream) -> CStore;

template <ClauseStorage CStore>
inline auto dimacs_cnf_parse(std::istream &&input_stream) -> CStore {
  // just a hack to allow rvalue input_streams as well
  return dimacs_cnf_parse<CStore>(input_stream);
}

auto dimacs_output_parse(std::istream &input_stream)
    -> std::optional<std::vector<Lit>>;

inline auto dimacs_output_parse(std::istream &&input_stream)
    -> std::optional<std::vector<Lit>> {
  return dimacs_output_parse(input_stream);
}

} // namespace peregrine::parse
