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

 TEST(ClauseTest, AddClausesLValue) {
 	auto clause_storage = std::make_unique<ClauseStoreDefault>(4);

	std::vector<std::vector<Lit>> clauses = {
		{ Lit{1}, Lit{-2}, Lit{-3} },
		{ Lit{-1}, Lit{2}, Lit{-3} },
		{ Lit{-1}, Lit{-2}, Lit{3} }
	};

	for (auto& clause : clauses) {
		clause_storage->add_clause(clause);
	}
 
 	EXPECT_EQ(clause_storage->num_clauses(), 3);

	for (int clause_idx = 0; clause_idx < clauses.size(); clause_idx++) {
		const auto& clause = clause_storage->get_clause(clause_idx);
		EXPECT_EQ(clause.literals, clauses[clause_idx]);
	}
 }

 TEST(ClauseTest, AddClausesRValue) {
 	auto clause_storage = std::make_unique<ClauseStoreDefault>(4);

	std::vector<std::vector<Lit>> clauses = {
		{ Lit{1}, Lit{-2}, Lit{-3} },
		{ Lit{-1}, Lit{2}, Lit{-3} },
		{ Lit{-1}, Lit{-2}, Lit{3} }
	};

	auto clauses_copy = clauses;

	for (auto& clause : clauses_copy) {
		clause_storage->add_clause(std::move(clause));
	}
 
 	EXPECT_EQ(clause_storage->num_clauses(), 3);

	for (int clause_idx = 0; clause_idx < clauses.size(); clause_idx++) {
		const auto& clause = clause_storage->get_clause(clause_idx);
		EXPECT_EQ(clause.literals, clauses[clause_idx]);
	}
 }

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
