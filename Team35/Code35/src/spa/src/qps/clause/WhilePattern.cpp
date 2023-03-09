#include "WhilePattern.h"

#include <list>

WhilePattern::WhilePattern(std::unique_ptr<PQLToken> first,
                             std::string ident) : OneArgClause(std::move(first)), ident(ident) {
    validateArgs();
}

std::unique_ptr<Result> WhilePattern::evaluate(PKBReader *db) {

    throw std::runtime_error("WhilePattern.cpp");
}

void WhilePattern::validateArgs() {
    Synonym* synonym = dynamic_cast<Synonym*>(first.get());
    if (synonym != nullptr && !(synonym->de == Synonym::DesignEntity::PROCEDURE ||
            synonym->de == Synonym::DesignEntity::VARIABLE ||
            synonym->de == Synonym::DesignEntity::CONSTANT)) {
        throw SemanticException();
    }
}

bool WhilePattern::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const WhilePattern *>(&rhs);
    return pRhs != nullptr && equal(*pRhs) && ident == pRhs->ident;
}
