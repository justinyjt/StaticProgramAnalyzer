#include <vector>

#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/clause_optimiser/types.h"
#include "MockOptimisableClause.h"
#include "qps/clause_optimiser/ClauseOptimiser.h"

TEST_CASE("1. test getClauseEvaluationOrder() method") {
    SECTION("1.1. no clause") {
        std::vector<std::unique_ptr<OptimisableClause>> clauses;
        ClauseOptimiser clause_optimiser(clauses);
        ClauseIndexList actual_clause_evaluation_order = clause_optimiser.getClauseEvaluationOrder();
        ClauseIndexList expected_clause_evaluation_order;
        requireEqualRef(actual_clause_evaluation_order, expected_clause_evaluation_order);
    }

    SECTION("1.2. one clause") {
        std::vector<std::unique_ptr<OptimisableClause>> clauses;
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::Low, HEADER_SET()));
        ClauseOptimiser clause_optimiser(clauses);
        ClauseIndexList actual_clause_evaluation_order = clause_optimiser.getClauseEvaluationOrder();
        ClauseIndexList expected_clause_evaluation_order = {0};
        requireEqualRef(actual_clause_evaluation_order, expected_clause_evaluation_order);
    }

    SECTION("1.3. clauses with no headers - prioritise LOW complexity") {
        std::vector<std::unique_ptr<OptimisableClause>> clauses;
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::High,
                                                                     HEADER_SET()));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::Medium,
                                                                     HEADER_SET()));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::Low, HEADER_SET()));
        ClauseOptimiser clause_optimiser(clauses);
        ClauseIndexList actual_clause_evaluation_order = clause_optimiser.getClauseEvaluationOrder();
        ClauseIndexList expected_clause_evaluation_order = {2, 1, 0};
        requireEqualRef(actual_clause_evaluation_order, expected_clause_evaluation_order);
    }

    SECTION("1.4. prioritise no header clause") {
        std::vector<std::unique_ptr<OptimisableClause>> clauses;
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::High,
                                                                     HEADER_SET{"0", "1", "2"}));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::Medium,
                                                                     HEADER_SET{"0", "1", "2"}));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::Low,
                                                                     HEADER_SET{"0", "1", "2"}));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::High,
                                                                     HEADER_SET()));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::Medium,
                                                                     HEADER_SET()));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::Low, HEADER_SET()));

        ClauseOptimiser clause_optimiser(clauses);
        ClauseIndexList actual_clause_evaluation_order = clause_optimiser.getClauseEvaluationOrder();
        ClauseIndexList expected_clause_evaluation_order = {5, 4, 3, 2, 1, 0};
        requireEqualRef(actual_clause_evaluation_order, expected_clause_evaluation_order);
    }

    SECTION("1.5. prioritise complexity and then low number of headers") {
        std::vector<std::unique_ptr<OptimisableClause>> clauses;
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::High,
                                                                     HEADER_SET{"0", "1", "2"}));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::Medium,
                                                                     HEADER_SET{"0", "1"}));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::Low,
                                                                     HEADER_SET{"0"}));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::High,
                                                                     HEADER_SET{"0", "1", "2", "3"}));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::Medium,
                                                                     HEADER_SET{"0", "1", "2"}));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::Low,
                                                                     HEADER_SET{"0", "1"}));

        ClauseOptimiser clause_optimiser(clauses);
        ClauseIndexList actual_clause_evaluation_order = clause_optimiser.getClauseEvaluationOrder();
        ClauseIndexList expected_clause_evaluation_order = {2, 5, 1, 4, 0, 3};
        requireEqualRef(actual_clause_evaluation_order, expected_clause_evaluation_order);
    }
}

TEST_CASE("2. test getClauseGroups() method") {
    SECTION("2.1. no clause") {
        std::vector<std::unique_ptr<OptimisableClause>> clauses;
        ClauseOptimiser clause_optimiser(clauses);
        ClauseGroupList actual_clause_groups = clause_optimiser.getClauseGroups();
        ClauseGroupList expected_clause_groups;
        requireEqualRef(actual_clause_groups, expected_clause_groups);
    }

    SECTION("2.2. clauses with no headers - give no groups") {
        std::vector<std::unique_ptr<OptimisableClause>> clauses;
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::High,
                                                                     HEADER_SET()));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::Medium,
                                                                     HEADER_SET()));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::Low, HEADER_SET()));
        ClauseOptimiser clause_optimiser(clauses);
        ClauseGroupList actual_clause_groups = clause_optimiser.getClauseGroups();
        ClauseGroupList expected_clause_groups;
        requireEqualRef(actual_clause_groups, expected_clause_groups);
    }

    SECTION("2.3. clauses with headers") {
        std::vector<std::unique_ptr<OptimisableClause>> clauses;
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::High,
                                                                     HEADER_SET{"0", "1", "2"}));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::Medium,
                                                                     HEADER_SET{"0", "1", "2"}));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::Low,
                                                                     HEADER_SET{"0", "1", "2"}));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::High,
                                                                     HEADER_SET{"0", "1", "2", "3"}));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::Medium,
                                                                     HEADER_SET{"0", "1", "2"}));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::Low,
                                                                     HEADER_SET{"0", "1"}));

        ClauseOptimiser clause_optimiser(clauses);
        ClauseGroupList actual_clause_groups = clause_optimiser.getClauseGroups();
        ClauseGroupList expected_clause_groups;
        expected_clause_groups.emplace_back(ClauseIndexList{0, 1, 2, 3, 4, 5}, HEADER_SET{"0", "1", "2", "3"});
        requireSameVectorNoOrder(actual_clause_groups[0].getClauseJoinOrder(),
                                 expected_clause_groups[0].getClauseJoinOrder());
        requireEqualRef(actual_clause_groups[0].getGroupHeaders(), expected_clause_groups[0].getGroupHeaders());
    }

    SECTION("2.4. filter out clauses with no headers - give 1 group") {
        std::vector<std::unique_ptr<OptimisableClause>> clauses;
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::High,
                                                                     HEADER_SET()));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::Medium,
                                                                     HEADER_SET{"0", "1", "2"}));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::Medium,
                                                                     HEADER_SET{"0", "3"}));
        clauses.emplace_back(std::make_unique<MockOptimisableClause>(OptimisableClause::Complexity::Low, HEADER_SET()));
        ClauseOptimiser clause_optimiser(clauses);
        ClauseGroupList actual_clause_groups = clause_optimiser.getClauseGroups();
        ClauseGroupList expected_clause_groups = {ClauseGroup(ClauseIndexList{1, 2}, HEADER_SET{"0", "1", "2", "3"})};
        requireSameVectorNoOrder(actual_clause_groups[0].getClauseJoinOrder(),
                                 expected_clause_groups[0].getClauseJoinOrder());
        requireEqualRef(actual_clause_groups[0].getGroupHeaders(), expected_clause_groups[0].getGroupHeaders());
    }
}
