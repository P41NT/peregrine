#include <gtest/gtest.h>

#include "clauses.hpp"
#include "types.hpp"

using namespace peregrine::core;
using namespace peregrine::clauses;


TEST(ClauseTest, FitsTypeOfClauseStorage) {
	static_assert(ClauseStorage<ClauseStoreDefault>);
	SUCCEED();
}

TEST(ClauseTest, GetNumVars) {
	auto clause_storage1 = std::make_unique<ClauseStoreDefault>(10);
	EXPECT_EQ(clause_storage1->num_vars(), 10);

	auto clause_storage2 = std::make_unique<ClauseStoreDefault>(20);
	EXPECT_EQ(clause_storage2->num_vars(), 20);

	auto clause_storage3 = std::make_unique<ClauseStoreDefault>(5);
	EXPECT_EQ(clause_storage3->num_vars(), 5);
}

 //TEST(ClauseTest, AddClauses) {
 //	auto clause_storage = std::make_unique<ClauseStoreDefault>(4);
 //
 //	//auto lits_rval = std::vector<Lit>{ Lit{1}, Lit{2}, Lit{3} };
 //	//clause_storage->add_clause(std::vector<Lit>{ Lit{1}, Lit{2}, Lit{3} }); // rvalue std::vector
 //
 //	std::vector<Lit> lits = { Lit{1}, Lit{-2}, Lit{-3} };
 //	clause_storage->add_clause(lits); // lvalue std::vector
 //
 //	EXPECT_EQ(clause_storage->num_clauses(), 2);
 //
 //	auto clause0 = clause_storage->get_clause(0);
 //	ASSERT_EQ(clause0.literals.size(), lits.size());
 //
 //	//auto clause1 = clause_storage->get_clause(1);
 //	//ASSERT_EQ(clause1.literals.size(), lits_rval.size());
 //
 //	//for (int lits_idx = 0; lits_idx < lits_rval.size(); lits_idx++) {
 //	//	EXPECT_EQ(lits_rval[lits_idx], clause0.literals[lits_idx]);
 //	//}
 //
 //	for (int lits_idx = 0; lits_idx < lits.size(); lits_idx++) {
 //		EXPECT_EQ(lits[lits_idx], clause0.literals[lits_idx]);
 //	}
 //}

TEST(ClauseTest, ClauseIsSatisfiedTest) {
	auto clause_storage0 = std::make_unique<ClauseStoreDefault>(4);

	std::vector<std::vector<Lit>> clauses0 = {
		{ Lit{1}, Lit{-2}, Lit{3} },
		{ Lit{-1}, Lit{2}, Lit{4} },
		{ Lit{-1}, Lit{3}, Lit{-4} },
	};

	auto assignment0 = std::make_unique<Assignment>(4);
	assignment0->new_level();
	assignment0->enqueue(Lit{ -1 }, 0);
	assignment0->enqueue(Lit{ 3 }, 0);
	assignment0->enqueue(Lit{ 2 }, 0);
	assignment0->enqueue(Lit{ 4 }, 0);

	for (auto& clause : clauses0) {
		clause_storage0->add_clause(std::move(clause));
	}

	EXPECT_EQ(clause_storage0->is_satisfied(*assignment0), true);

	assignment0->enqueue(Lit{ -1 }, 0);
	assignment0->enqueue(Lit{ 2 }, 0);
	assignment0->enqueue(Lit{ -3 }, 0);

	EXPECT_EQ(clause_storage0->is_satisfied(*assignment0), false);
}
