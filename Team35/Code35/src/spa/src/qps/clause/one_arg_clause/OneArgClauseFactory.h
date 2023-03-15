#pragma once

#include "OneArgClause.h"

class OneArgClauseFactory {
public:
    static std::unique_ptr<OneArgClause> createWhilePatternClause(std::unique_ptr<PQLToken>, std::string);
    static std::unique_ptr<OneArgClause> createIfPatternClause(std::unique_ptr<PQLToken>, std::string);
};
