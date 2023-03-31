#include <algorithm>

#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/clause/OptimisableClause.h"
#include "qps/clause_optimiser/ClauseGroupGraph.h"
#include "qps/clause_optimiser/types.h"

void assertCorrectOrder(const std::unordered_map<ClauseIndex, HEADER_SET> &validator_map,
                        const ClauseIndexList &actual_clause_groups_indices) {
    HEADER_SET clause_header_set;
    ClauseIndex first_clause_index = actual_clause_groups_indices[0];
    clause_header_set.insert(validator_map.at(first_clause_index).begin(),
                             validator_map.at(first_clause_index).end());
    for (int i = 1; i < actual_clause_groups_indices.size(); i++) {
        ClauseIndex clause_index = actual_clause_groups_indices[i];
        requireTrue(hasIntersection(clause_header_set, validator_map.at(clause_index)));
        clause_header_set.insert(validator_map.at(clause_index).begin(), validator_map.at(clause_index).end());
    }
}

TEST_CASE("1. test getClauseGroupsIndices() method") {
    SECTION("1.1. empty clause group") {
        ClauseGroupGraph clause_group_graph;
        std::vector<ClauseIndexList> actual_clause_groups_indices = clause_group_graph.getClauseGroupsIndices();
        std::vector<ClauseIndexList> expected_clause_groups_indices;
        requireEqualRef(expected_clause_groups_indices, actual_clause_groups_indices);
    }

    SECTION("1.2. one clause group") {
        ClauseGroupGraph clause_group_graph;
        clause_group_graph.addClauseHeaderRelations(0, {"a", "b", "c"});
        clause_group_graph.addClauseHeaderRelations(1, {"a", "b", "c"});
        clause_group_graph.addClauseHeaderRelations(2, {"b", "c", "d"});
        clause_group_graph.addClauseHeaderRelations(3, {"c", "d", "e"});
        std::vector<ClauseIndexList> actual_clause_groups_indices = clause_group_graph.getClauseGroupsIndices();
        std::sort(actual_clause_groups_indices[0].begin(), actual_clause_groups_indices[0].end());
        std::vector<ClauseIndexList> expected_clause_groups_indices = {{0, 1, 2, 3}};
        requireEqualRef(expected_clause_groups_indices, actual_clause_groups_indices);
    }

    SECTION("1.3. two clause groups") {
        ClauseGroupGraph clause_group_graph;
        clause_group_graph.addClauseHeaderRelations(0, {"a", "b", "c"});
        clause_group_graph.addClauseHeaderRelations(1, {"a", "b", "c"});
        clause_group_graph.addClauseHeaderRelations(2, {"b", "c", "d"});
        clause_group_graph.addClauseHeaderRelations(3, {"c", "d", "e"});
        clause_group_graph.addClauseHeaderRelations(4, {"p", "q", "r"});
        clause_group_graph.addClauseHeaderRelations(5, {"s", "r", "t"});
        std::vector<ClauseIndexList> actual_clause_groups_indices = clause_group_graph.getClauseGroupsIndices();
        std::unordered_map<ClauseIndex, HEADER_SET> clause_header_map = {
            {0, {"a", "b", "c"}},
            {1, {"a", "b", "c"}},
            {2, {"b", "c", "d"}},
            {3, {"c", "d", "e"}},
            {4, {"p", "q", "r"}},
            {5, {"s", "t", "q"}},
        };
        assertCorrectOrder(clause_header_map, actual_clause_groups_indices[0]);
        assertCorrectOrder(clause_header_map, actual_clause_groups_indices[1]);
        std::vector<ClauseIndexList> expected_clause_groups_indices = {{0, 1, 2, 3}, {4, 5}};
        for (auto &clause_group_indices : actual_clause_groups_indices) {
            std::sort(clause_group_indices.begin(), clause_group_indices.end());
        }
        requireEqual(expected_clause_groups_indices.size(), actual_clause_groups_indices.size());
        for (auto &clause_group_indices : actual_clause_groups_indices) {
            bool isFound = false;
            for (auto &expected_clause_group_indices : expected_clause_groups_indices) {
                if (clause_group_indices == expected_clause_group_indices) {
                    isFound = true;
                }
            }
            requireTrue(isFound);
        }
        for (auto &expected_clause_group_indices : expected_clause_groups_indices) {
            bool isFound = false;
            for (auto &clause_group_indices : actual_clause_groups_indices) {
                if (clause_group_indices == expected_clause_group_indices) {
                    isFound = true;
                }
            }
            requireTrue(isFound);
        }
    }

    SECTION("1.3. line graph to check order") {
        ClauseGroupGraph clause_group_graph;
        clause_group_graph.addClauseHeaderRelations(0, {"a", "b"});
        clause_group_graph.addClauseHeaderRelations(1, {"b", "c"});
        clause_group_graph.addClauseHeaderRelations(2, {"c", "d"});
        clause_group_graph.addClauseHeaderRelations(3, {"d", "e"});
        clause_group_graph.addClauseHeaderRelations(4, {"e", "f"});
        clause_group_graph.addClauseHeaderRelations(5, {"f", "g"});
        clause_group_graph.addClauseHeaderRelations(6, {"g", "h"});
        clause_group_graph.addClauseHeaderRelations(7, {"h", "i"});
        clause_group_graph.addClauseHeaderRelations(8, {"i", "j"});
        clause_group_graph.addClauseHeaderRelations(9, {"j", "k"});
        std::unordered_map<ClauseIndex, HEADER_SET> clause_header_map = {
            {0, {"a", "b"}},
            {1, {"b", "c"}},
            {2, {"c", "d"}},
            {3, {"d", "e"}},
            {4, {"e", "f"}},
            {5, {"f", "g"}},
            {6, {"g", "h"}},
            {7, {"h", "i"}},
            {8, {"i", "j"}},
            {9, {"j", "k"}},
        };
        std::vector<ClauseIndexList> actual_clause_groups_indices = clause_group_graph.getClauseGroupsIndices();
        assertCorrectOrder(clause_header_map, actual_clause_groups_indices[0]);
        for (auto &clause_group_indices : actual_clause_groups_indices) {
            std::sort(clause_group_indices.begin(), clause_group_indices.end());
        }
        std::vector<ClauseIndexList> expected_clause_groups_indices = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
        requireEqualRef(expected_clause_groups_indices, actual_clause_groups_indices);
    }

    SECTION("1.4. empty header set") {
        ClauseGroupGraph clause_group_graph;
        clause_group_graph.addClauseHeaderRelations(0, {});
        clause_group_graph.addClauseHeaderRelations(1, {});
        clause_group_graph.addClauseHeaderRelations(2, {});
        clause_group_graph.addClauseHeaderRelations(3, {});
        clause_group_graph.addClauseHeaderRelations(4, {"a", "b", "c"});
        clause_group_graph.addClauseHeaderRelations(5, {});
        clause_group_graph.addClauseHeaderRelations(6, {});
        clause_group_graph.addClauseHeaderRelations(7, {});
        clause_group_graph.addClauseHeaderRelations(8, {});
        clause_group_graph.addClauseHeaderRelations(9, {});
        std::vector<ClauseIndexList> actual_clause_groups_indices = clause_group_graph.getClauseGroupsIndices();
        std::vector<ClauseIndexList> expected_clause_groups_indices = {{4}};
        requireEqualRef(expected_clause_groups_indices, actual_clause_groups_indices);
    }

    SECTION("1.5. line graph to check order - shuffle clause indices") {
        ClauseGroupGraph clause_group_graph;
        clause_group_graph.addClauseHeaderRelations(6, {"a", "b"});
        clause_group_graph.addClauseHeaderRelations(3, {"b", "c"});
        clause_group_graph.addClauseHeaderRelations(1, {"c", "d"});
        clause_group_graph.addClauseHeaderRelations(2, {"d", "e"});
        clause_group_graph.addClauseHeaderRelations(4, {"e", "f"});
        clause_group_graph.addClauseHeaderRelations(5, {"f", "g"});
        clause_group_graph.addClauseHeaderRelations(9, {"g", "h"});
        clause_group_graph.addClauseHeaderRelations(7, {"h", "i"});
        clause_group_graph.addClauseHeaderRelations(8, {"i", "j"});
        clause_group_graph.addClauseHeaderRelations(0, {"j", "k"});
        std::unordered_map<ClauseIndex, HEADER_SET> clause_header_map = {
            {6, {"a", "b"}},
            {3, {"b", "c"}},
            {1, {"c", "d"}},
            {2, {"d", "e"}},
            {4, {"e", "f"}},
            {5, {"f", "g"}},
            {9, {"g", "h"}},
            {7, {"h", "i"}},
            {8, {"i", "j"}},
            {0, {"j", "k"}},
        };
        std::vector<ClauseIndexList> actual_clause_groups_indices = clause_group_graph.getClauseGroupsIndices();
        assertCorrectOrder(clause_header_map, actual_clause_groups_indices[0]);
        for (auto &clause_group_indices : actual_clause_groups_indices) {
            std::sort(clause_group_indices.begin(), clause_group_indices.end());
        }
        std::vector<ClauseIndexList> expected_clause_groups_indices = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}};
        requireEqualRef(expected_clause_groups_indices, actual_clause_groups_indices);
    }
}
