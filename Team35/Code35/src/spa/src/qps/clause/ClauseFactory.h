#pragma once

#include <memory>
#include <string>
#include <utility>

#include "qps/clause/two_arg_clause/TwoArgClause.h"
#include "qps/clause/one_arg_clause/OneArgClause.h"

class ClauseFactory {
 public:
    static std::unique_ptr<TwoArgClause> createAssignPatternClause(std::unique_ptr<PQLToken>,
            std::unique_ptr<PQLToken>, std::string);
    static std::unique_ptr<TwoArgClause> createClause(std::unique_ptr<PQLToken>,
            std::unique_ptr<PQLToken>, std::string);
    static std::unique_ptr<OneArgClause> createIfPatternClause(std::unique_ptr<PQLToken>, std::string);
    static std::unique_ptr<OneArgClause> createWhilePatternClause(std::unique_ptr<PQLToken>, std::string);
};
