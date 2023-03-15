#include "OneArgClauseFactory.h"
#include "WhilePattern.h"
#include "IfPattern.h"

std::unique_ptr<OneArgClause> OneArgClauseFactory::createWhilePatternClause(std::unique_ptr<PQLToken> token1, std::string patternStr) {
    std::unique_ptr<OneArgClause> a = std::make_unique<WhilePattern>(std::move(token1), patternStr);
    return std::move(a);
}

std::unique_ptr<OneArgClause> OneArgClauseFactory::createIfPatternClause(std::unique_ptr<PQLToken> token1, std::string patternStr) {
    std::unique_ptr<OneArgClause> a = std::make_unique<IfPattern>(std::move(token1), patternStr);
    return std::move(a);
}