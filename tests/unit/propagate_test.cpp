#include <gtest/gtest.h>

#include "assignment.hpp"
#include "clauses.hpp"
#include "propagator.hpp"
#include "types.hpp"

using namespace peregrine::core;
using namespace peregrine::clauses;
using namespace peregrine::propagate;

class PropagateTest : public ::testing::Test {
protected:
  std::unique_ptr<ClauseStoreDefault> clause_storage;
  std::unique_ptr<Assignment> assignment;

  size_t numVariables = 10;

  void SetUp() override {
    clause_storage = std::make_unique<ClauseStoreDefault>(numVariables);
    assignment = std::make_unique<Assignment>(numVariables);
  }
};

TEST_F(PropagateTest, PropagateUnitClause) {
  clause_storage->add_clause(std::vector<Lit>({Lit{1}, Lit{2}}));
  auto propagator = SimpleBCP<ClauseStoreDefault>(*clause_storage);

  assignment->enqueue(Var{2}, LBool::FALSE, 0);
  propagator.propagate(*assignment);

  EXPECT_EQ(assignment->getVar(Var{1}), LBool::TRUE);
}

TEST_F(PropagateTest, PropagateMultipleUnitClauses) {
  clause_storage->add_clause(std::vector<Lit>({Lit{1}, Lit{3}}));
  clause_storage->add_clause(std::vector<Lit>({Lit{-2}, Lit{3}}));

  auto propagator = SimpleBCP<ClauseStoreDefault>(*clause_storage);
  assignment->enqueue(Var{3}, LBool::FALSE, 0);

  propagator.propagate(*assignment);

  EXPECT_EQ(assignment->getVar(Var{1}), LBool::TRUE);
  EXPECT_EQ(assignment->getVar(Var{2}), LBool::FALSE);
}

TEST_F(PropagateTest, PropagateUnitClauseConflictsWithMultipleClauses) {
  clause_storage->add_clause(std::vector<Lit>({Lit{1}, Lit{2}}));
  clause_storage->add_clause(std::vector<Lit>({Lit{-1}, Lit{3}}));

  auto propagator = SimpleBCP<ClauseStoreDefault>(*clause_storage);

  assignment->enqueue(Var{2}, LBool::FALSE, 0);
  assignment->enqueue(Var{3}, LBool::FALSE, 0);

  auto conflict = propagator.propagate(*assignment);

  EXPECT_TRUE(conflict.has_value());
  EXPECT_TRUE(conflict.value() == 0 || conflict.value() == 1);
}

TEST_F(PropagateTest, PropagateMultipleLevelUnitClauses) {
  std::vector<std::vector<Lit>> clauses = {
      {Lit{1}, Lit{2}, Lit{3}},   {Lit{-3}, Lit{2}, Lit{4}},
      {Lit{-4}, Lit{-3}, Lit{5}}, {Lit{-5}, Lit{6}, Lit{7}},
      {Lit{-6}, Lit{7}, Lit{8}},
  };

  for (auto &clause : clauses) {
    clause_storage->add_clause(clause);
  }

  auto propagator = SimpleBCP<ClauseStoreDefault>(*clause_storage);

  assignment->new_level();
  assignment->enqueue(Var{1}, LBool::FALSE, 0);
  assignment->enqueue(Var{2}, LBool::FALSE, 0);

  auto conflict = propagator.propagate(*assignment);

  ASSERT_TRUE(conflict.has_value() == false);
  EXPECT_TRUE(assignment->getVar(Var{3}) == LBool::TRUE);
  EXPECT_TRUE(assignment->getVar(Var{4}) == LBool::TRUE);
  EXPECT_TRUE(assignment->getVar(Var{5}) == LBool::TRUE);

  assignment->new_level();
  assignment->enqueue(Var{7}, LBool::FALSE, 0);

  auto conflict2 = propagator.propagate(*assignment);
  ASSERT_TRUE(conflict2.has_value() == false);
  EXPECT_TRUE(assignment->getVar(Var{6}) == LBool::TRUE);
  EXPECT_TRUE(assignment->getVar(Var{8}) == LBool::TRUE);
}