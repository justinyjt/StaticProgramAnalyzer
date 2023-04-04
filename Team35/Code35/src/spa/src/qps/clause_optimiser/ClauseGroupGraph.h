#pragma once

#include <vector>

#include "commons/graph/Graph.h"
#include "qps/clause/OptimisableClause.h"
#include "qps/clause_optimiser/types.h"

struct ClauseGroupGraphNode {
    enum class Type {
        CLAUSE, HEADER
    };

    const Type type;
    const HEADER header;
    const ClauseIndex clause_index;

    explicit ClauseGroupGraphNode(const ClauseIndex &clause_index);
    explicit ClauseGroupGraphNode(HEADER header);
    bool operator==(const ClauseGroupGraphNode &rhs) const;
    bool operator!=(const ClauseGroupGraphNode &rhs) const;
};

namespace std {
template<>
struct hash<ClauseGroupGraphNode> {
    std::size_t operator()(ClauseGroupGraphNode const &data) const noexcept {
        if (data.type == ClauseGroupGraphNode::Type::CLAUSE) {
            return std::hash<ClauseIndex>()(data.clause_index);
        } else {
            return std::hash<HEADER>()(data.header);
        }
    }
};
}  // namespace std

class ClauseGroupGraph : Graph<ClauseGroupGraphNode> {
 public:
    ClauseGroupGraph() = default;
    ClauseGroupGraph(const ClauseGroupGraph &other) = default;
    ClauseGroupGraph(ClauseGroupGraph &&other) noexcept = default;
    ClauseGroupGraph &operator=(const ClauseGroupGraph &other) = default;
    ClauseGroupGraph &operator=(ClauseGroupGraph &&other) noexcept = default;
    ~ClauseGroupGraph() = default;

    void addClauseHeaderRelations(const ClauseIndex &clause_index, const HEADER_SET &headers);
    std::vector<ClauseIndexList> getClauseGroupsIndices() const;

 private:
    ClauseIndexSet clause_indices_;
    ClauseIndexSet getNeighboringClauseIndices(const ClauseIndex &clause_index) const;
    const ClauseIndex &getClauseIndex(const Index &clause_node_index) const;
    ClauseIndexList getSingleClauseGroupIndices(const ClauseIndex &clause_index,
                                                ClauseIndexSet &visited_clause_indices) const;
};
