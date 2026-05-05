#include <gtest/gtest.h>

#include "dpll.hpp"
#include "parser.hpp"

using namespace peregrine::core;
using namespace peregrine::clauses;
using namespace peregrine::solver;
using namespace peregrine::parse;
using namespace peregrine::propagate;

void test_dimacs_testname(std::string test_name, bool result);

TEST(DPLL_Test, UN20) { test_dimacs_testname("un20", true); }
TEST(DPLL_Test, SimpleHand) { test_dimacs_testname("simple_hand", true); }
//TEST(DPLL_Test, AIM100) { test_dimacs_testname("aim100", false); }

void test_dimacs_testname(std::string test_name, bool result) {

  std::string input_filename =
      std::format("C:\\Users\\shawn\\source\\repos\\peregrine\\sat_"
                  "files\\dimacs_cnf\\inputs\\{}.cnf",
                  test_name);

  dpll::DPLLSolverDefault<ClauseStoreDefault, SimpleBCP<ClauseStoreDefault>>
      dpll_solver(input_filename);

  std::optional<std::vector<Lit>> solver_output = dpll_solver.solve();

  ASSERT_EQ(solver_output.has_value(), result);
}
