#include "ClauseOptimiser.h"

#include <algorithm>

#include "ClauseGroupGraph.h"

ClauseOptimiser::ClauseOptimiser(const std::vector<std::unique_ptr<OptimisableClause>> &clauses) : clauses_(clauses) {}

ClauseIndexList ClauseOptimiser::getClauseEvaluationOrder() const {
    ClauseIndexList clause_evaluation_order;
    for (ClauseIndex i = 0; i < clauses_.size(); ++i) {
        clause_evaluation_order.push_back(i);
    }
    std::sort(clause_evaluation_order.begin(),
              clause_evaluation_order.end(),
              [this](const ClauseIndex &lhs, const ClauseIndex &rhs) {
                return this->compareClauseIndex(lhs, rhs);
              });
    return clause_evaluation_order;
}

bool ClauseOptimiser::compareClauseIndex(const ClauseIndex &lhs, const ClauseIndex &rhs) const {
    if (clauses_[lhs]->getHeaderCount() == 0 && clauses_[rhs]->getHeaderCount() == 0) {
        return clauses_[lhs]->getComplexity() < clauses_[rhs]->getComplexity();
    } else if (clauses_[lhs]->getHeaderCount() == 0) {
        return true;
    } else if (clauses_[rhs]->getHeaderCount() == 0) {
        return false;
    }
    if (clauses_[lhs]->getComplexity() != clauses_[rhs]->getComplexity()) {
        return clauses_[lhs]->getComplexity() < clauses_[rhs]->getComplexity();
    }
    return clauses_[lhs]->getHeaderCount() < clauses_[rhs]->getHeaderCount();
}

ClauseGroupList ClauseOptimiser::getClauseGroups() const {
    ClauseGroupGraph clause_group_graph;
    for (int i = 0; i < clauses_.size(); ++i) {
        clause_group_graph.addClauseHeaderRelations(i, clauses_[i]->getHeaders());
    }
    auto clause_groups_indices_list = clause_group_graph.getClauseGroupsIndices();
    ClauseGroupList clause_groups;
    for (const auto &clause_group_indices : clause_groups_indices_list) {
        HEADER_SET headers;
        for (const auto &clause_index : clause_group_indices) {
            auto clause_headers = clauses_[clause_index]->getHeaders();
            headers.insert(clause_headers.begin(), clause_headers.end());
        }
        clause_groups.emplace_back(clause_group_indices, headers);
    }
    return clause_groups;
}
