#pragma once

#include <memory>
#include <string>
#include <utility>

#include "qps/clause/TwoArgClause/TwoArgClause.h"

class TwoArgClauseFactory {
 public:
    static std::unique_ptr<TwoArgClause> createAssignPatternClause(std::unique_ptr<PQLToken>,
            std::unique_ptr<PQLToken>, std::string);
    static std::unique_ptr<TwoArgClause> createClause(std::unique_ptr<PQLToken>,
            std::unique_ptr<PQLToken>, std::string);
};
