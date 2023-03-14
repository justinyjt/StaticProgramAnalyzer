#include <list>

#include "IfPattern.h"

IfPattern::IfPattern(std::unique_ptr<PQLToken> first,
                           std::string ident) : OneArgClause(std::move(first)), ident(ident) {
    validateArgs();
}

std::unique_ptr<Result> IfPattern::evaluate(PKBReader *db) {
    throw std::runtime_error("IfPattern.cpp");
}

void IfPattern::validateArgs() {
    Synonym* synonym = dynamic_cast<Synonym*>(first.get());
    if (synonym != nullptr && !(synonym->de == Synonym::DesignEntity::PROCEDURE ||
            synonym->de == Synonym::DesignEntity::VARIABLE ||
            synonym->de == Synonym::DesignEntity::CONSTANT)) {
        throw SemanticException();
    }
}

bool IfPattern::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const IfPattern *>(&rhs);
    return pRhs != nullptr && equal(*pRhs) && ident == pRhs->ident;
}
