#pragma once

#include <optional>
#include <unordered_map>

#include "commons/graph/Graph.h"
#include "commons/types.h"

class AffectsGraph : public Graph<STMT_NUM> {
 public:
    AffectsGraph() = default;

    ~AffectsGraph() override = default;

    void addAffectsEdge(STMT_NUM first, STMT_NUM second);

    bool hasAffectsRelationship(STMT_NUM first, STMT_NUM second);

    const STMT_STMT_SET &getAllAffectsRelationships(bool is_transitive);

    void reset() override;

 private:
    STMT_STMT_SET direct_affects_set_;
    std::optional<STMT_STMT_SET> transitive_affects_set_;

    void populateAffectsRelationshipOfIndex(Index idx, STMT_STMT_SET &result) const;
};
