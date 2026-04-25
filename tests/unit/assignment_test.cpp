#include <gtest/gtest.h>

#include "assignment.hpp"
#include "types.hpp"

using namespace peregrine::clauses;
using namespace peregrine::core;

class AssignmentTest : public ::testing::Test {
protected:
	std::unique_ptr<Assignment> assignment;

	void SetUp() override {
		assignment = std::make_unique<Assignment>(10);
	}
};

TEST_F(AssignmentTest, UndefVariablesInitialized) {
	for (int i = 0; i < 10; i++) {
		Var v{ i };
		EXPECT_EQ(assignment->getVar(v), LBool::UNDEF);
	}
}

TEST_F(AssignmentTest, GetCurrentLevelIsInitializedToZero) {
	EXPECT_EQ(assignment->getCurrentLevel(), 0);
}

TEST_F(AssignmentTest, EnqueueSingleVariable) {
	Var v{ 0 };
	assignment->enqueue(v, LBool::TRUE, 0);
	EXPECT_EQ(assignment->getVar(v), LBool::TRUE);
}

TEST_F(AssignmentTest, EnqueueMultipleVariables) {
	assignment->enqueue(Var{ 0 }, LBool::TRUE, 0);
	assignment->enqueue(Var{ 1 }, LBool::FALSE, 0);
	assignment->enqueue(Var{ 2 }, LBool::TRUE, 0);

	EXPECT_EQ(assignment->getVar(Var{ 0 }), LBool::TRUE);
	EXPECT_EQ(assignment->getVar(Var{ 1 }), LBool::FALSE);
	EXPECT_EQ(assignment->getVar(Var{ 2 }), LBool::TRUE);
}

TEST_F(AssignmentTest, NewLevelIncrementsCurrentLevel) {
	EXPECT_EQ(assignment->getCurrentLevel(), 0);

	assignment->new_level();
	EXPECT_EQ(assignment->getCurrentLevel(), 1);

	assignment->new_level();
	EXPECT_EQ(assignment->getCurrentLevel(), 2);
}

TEST_F(AssignmentTest, AssignmentsAtDifferentLevels) {
	assignment->enqueue(Var{ 1 }, LBool::TRUE, 0);

	assignment->new_level();
	assignment->enqueue(Var{ 2 }, LBool::FALSE, 0);

	assignment->new_level();
	assignment->enqueue(Var{ 3 }, LBool::TRUE, 0);

	EXPECT_EQ(assignment->getCurrentLevel(), 2);
	EXPECT_EQ(assignment->getVar(Var{ 1 }), LBool::TRUE);
	EXPECT_EQ(assignment->getVar(Var{ 2 }), LBool::FALSE);
	EXPECT_EQ(assignment->getVar(Var{ 3 }), LBool::TRUE);
}

TEST_F(AssignmentTest, PopToLevelRestoresVariables) {
	assignment->enqueue(Var{ 1 }, LBool::TRUE, 0);

	assignment->new_level();
	assignment->enqueue(Var{ 2 }, LBool::FALSE, 0);
	assignment->enqueue(Var{ 3 }, LBool::TRUE, 0);

	assignment->pop_to_level(0);

	EXPECT_EQ(assignment->getCurrentLevel(), 0);
	EXPECT_EQ(assignment->getVar(Var{ 1 }), LBool::TRUE);
	EXPECT_EQ(assignment->getVar(Var{ 2 }), LBool::UNDEF);
	EXPECT_EQ(assignment->getVar(Var{ 3 }), LBool::UNDEF);
}

TEST_F(AssignmentTest, PopToLevelMultipleTimes) {
	assignment->enqueue(Var{ 0 }, LBool::TRUE, 0);

	assignment->new_level();
	assignment->enqueue(Var{ 1 }, LBool::FALSE, 0);

	assignment->new_level();
	assignment->enqueue(Var{ 2 }, LBool::TRUE, 0);
	assignment->enqueue(Var{ 3 }, LBool::FALSE, 0);

	assignment->pop_to_level(1);

	EXPECT_EQ(assignment->getCurrentLevel(), 1);
	EXPECT_EQ(assignment->getVar(Var{ 0 }), LBool::TRUE);
	EXPECT_EQ(assignment->getVar(Var{ 1 }), LBool::FALSE);
	EXPECT_EQ(assignment->getVar(Var{ 2 }), LBool::UNDEF);
	EXPECT_EQ(assignment->getVar(Var{ 3 }), LBool::UNDEF);
}

TEST_F(AssignmentTest, PopToLevelZero) {
	assignment->enqueue(Var{ 0 }, LBool::TRUE, 0);
	assignment->new_level();
	assignment->enqueue(Var{ 1 }, LBool::FALSE, 0);

	assignment->pop_to_level(0);

	EXPECT_EQ(assignment->getCurrentLevel(), 0);
	EXPECT_EQ(assignment->getVar(Var{ 0 }), LBool::TRUE);
	EXPECT_EQ(assignment->getVar(Var{ 1 }), LBool::UNDEF);
}
