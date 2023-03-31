#include "ClauseGroup.h"

#include <utility>

ClauseGroup::ClauseGroup(ClauseIndexList clause_join_order, const HEADER_SET &group_headers) :
    clause_join_order_(std::move(clause_join_order)),
    group_headers_(std::move(group_headers)) {}

const ClauseIndexList &ClauseGroup::getClauseJoinOrder() const {
    return this->clause_join_order_;
}

const HEADER_SET &ClauseGroup::getGroupHeaders() const {
    return this->group_headers_;
}

bool ClauseGroup::operator==(const ClauseGroup &rhs) const {
    return this->clause_join_order_ == rhs.clause_join_order_ &&
        this->group_headers_ == rhs.group_headers_;
}

bool ClauseGroup::operator!=(const ClauseGroup &rhs) const {
    return !(rhs == *this);
}
