#include "OptimisableClause.h"

OptimisableClause::OptimisableClause() : complexity_(Complexity::Low) {}

OptimisableClause::Complexity OptimisableClause::getComplexity() const {
    return this->complexity_;
}

void OptimisableClause::setComplexity(OptimisableClause::Complexity complexity) {
    this->complexity_ = complexity;
}
