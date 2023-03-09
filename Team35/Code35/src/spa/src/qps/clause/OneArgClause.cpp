#include "OneArgClause.h"

#include <cassert>

OneArgClause::OneArgClause(std::unique_ptr<PQLToken> first) : first(std::move(first)) {}

EntType OneArgClause::getEntType(Synonym::DesignEntity type) {
    switch (type) {
        case Synonym::DesignEntity::PROCEDURE:return EntType::Procedure;
        case Synonym::DesignEntity::VARIABLE:return EntType::Variable;
        case Synonym::DesignEntity::CONSTANT:return EntType::Constant;
        default:assert(false);
    }
}

bool OneArgClause::equal(const OneArgClause &rhs) const {
    return *first == *(rhs.first);
}