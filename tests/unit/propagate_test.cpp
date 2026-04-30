#include <gtest/gtest.h>

#include "propagator.hpp"
#include "assignment.hpp"
#include "types.hpp"
#include "clauses.hpp"

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
	clause_storage->add_clause(std::vector<Lit>({ Lit{1}, Lit{2} }));
	auto propagator = SimpleBCP<ClauseStoreDefault>(*clause_storage);

	assignment->enqueue(Var{ 2 }, LBool::FALSE, 0);
	propagator.propagate(*assignment);

	EXPECT_EQ(assignment->getVar(Var{ 1 }), LBool::TRUE);
}

TEST_F(PropagateTest, PropagateMultipleUnitClauses) {
	clause_storage->add_clause(std::vector<Lit>({ Lit{1}, Lit{3} }));
	clause_storage->add_clause(std::vector<Lit>({ Lit{-2}, Lit{3} }));

	auto propagator = SimpleBCP<ClauseStoreDefault>(*clause_storage);
	assignment->enqueue(Var{ 3 }, LBool::FALSE, 0);

	propagator.propagate(*assignment);

	EXPECT_EQ(assignment->getVar(Var{ 1 }), LBool::TRUE);
	EXPECT_EQ(assignment->getVar(Var{ 2 }), LBool::FALSE);
}

TEST_F(PropagateTest, PropagateUnitClauseConflictsWithMultipleClauses) {
	clause_storage->add_clause(std::vector<Lit>({ Lit{1}, Lit{2} }));
	clause_storage->add_clause(std::vector<Lit>({ Lit{-1}, Lit{3} }));

	auto propagator = SimpleBCP<ClauseStoreDefault>(*clause_storage);

	assignment->enqueue(Var{ 2 }, LBool::FALSE, 0);
	assignment->enqueue(Var{ 3 }, LBool::FALSE, 0);

	auto conflict = propagator.propagate(*assignment);

	EXPECT_TRUE(conflict.has_value());
	EXPECT_TRUE(conflict.value() == 0 || conflict.value() == 1);
}