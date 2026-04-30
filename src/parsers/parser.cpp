#include "../../include/parser.hpp"

#include <istream>
#include <string>

namespace peregrine::parse {

using namespace peregrine::clauses;

template <ClauseStorage CStore>
auto dimacs_cnf_parse(std::istream &input_stream) -> CStore {
  constexpr auto max_stream_size = std::numeric_limits<std::streamsize>::max();

  while (true) {
    std::string s;
    input_stream >> s;
    if (input_stream.eof() || input_stream.bad()) {
      break;
    } else if (s == "c") {
      input_stream.ignore(max_stream_size, '\n');
    } else if (s == "p") {
      std::string cnf_str;
      int num_vars = 0, num_clauses = 0;
      input_stream >> cnf_str >> num_vars >> num_clauses;

      CStore clause_store(num_vars);

      for (int clause_idx = 0; clause_idx < num_clauses; clause_idx++) {
        std::vector<Lit> current_clause;

        int literal = 0;
        while (input_stream >> literal) {
          if (literal == 0) {
            break;
          } else {
            Lit currVar = peregrine::core::VarToLit(Var{literal}, true);
            current_clause.push_back(currVar);
          }
        }
        clause_store.add_clause(std::move(current_clause));
      }

      return clause_store;
    }
  }
  return CStore(1);
}

template ClauseStoreDefault
dimacs_cnf_parse<ClauseStoreDefault>(std::istream &);

} // namespace peregrine::parse
