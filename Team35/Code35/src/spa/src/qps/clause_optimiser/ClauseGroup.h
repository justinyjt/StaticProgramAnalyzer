#pragma once

#include <vector>

#include "qps/clause/OptimisableClause.h"
#include "qps/clause_optimiser/types.h"

class ClauseGroup {
 public:
    ClauseGroup(ClauseIndexList clause_join_order, const HEADER_SET &group_headers);
    ClauseGroup(const ClauseGroup &other) = default;
    ClauseGroup(ClauseGroup &&other) noexcept = default;
    ClauseGroup &operator=(const ClauseGroup &other) = default;
    ClauseGroup &operator=(ClauseGroup &&other) noexcept = default;
    ~ClauseGroup() = default;
    bool operator==(const ClauseGroup &rhs) const;
    bool operator!=(const ClauseGroup &rhs) const;

    const ClauseIndexList &getClauseJoinOrder() const;
    const HEADER_SET &getGroupHeaders() const;

 private:
    ClauseIndexList clause_join_order_;
    HEADER_SET group_headers_;
};

typedef std::vector<ClauseGroup> ClauseGroupList;
