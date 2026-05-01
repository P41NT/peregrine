#include <gtest/gtest.h>

#include "dpll.hpp"
#include "parser.hpp"

using namespace peregrine::core;
using namespace peregrine::clauses;
using namespace peregrine::solver;
using namespace peregrine::parse;
using namespace peregrine::propagate;

void test_dimacs_testname(std::string test_name);

TEST(DPLL_Test, AIM100) { test_dimacs_testname("aim100"); }

TEST(DPLL_Test, ToughSat1) { test_dimacs_testname("toughtsat1"); }

void test_dimacs_testname(std::string test_name) {

  std::string input_filename =
      std::format("../../sat_files/dimacs_cnf/inputs/{}.cnf", test_name);
  std::string output_filename =
      std::format("../../sat_files/dimacs_cnf/outputs/{}.out", test_name);

  dpll::DPLLSolverDefault<ClauseStoreDefault, SimpleBCP<ClauseStoreDefault>>
      dpll_solver(input_filename);

  std::optional<std::vector<Lit>> solver_output = dpll_solver.solve();
  std::optional<std::vector<Lit>> true_output =
      dimacs_output_parse(std::ifstream(output_filename));

  EXPECT_EQ(solver_output, true_output);
}
