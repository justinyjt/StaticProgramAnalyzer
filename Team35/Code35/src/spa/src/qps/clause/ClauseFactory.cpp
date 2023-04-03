#include "ClauseFactory.h"
#include "qps/clause/two_arg_clause/StmtEntClause.h"
#include "qps/clause/two_arg_clause/EntEntClause.h"
#include "qps/clause/two_arg_clause/StmtStmtClause.h"
#include "qps/clause/two_arg_clause/WithEntClause.h"
#include "qps/clause/two_arg_clause/WithNumClause.h"
#include "qps/clause/two_arg_clause/AssignPatternClause.h"
#include "qps/query_parser/helper.h"

std::unique_ptr<TwoArgClause> ClauseFactory::createAssignPatternClause(std::unique_ptr<PQLToken> token1,
                                                                       std::unique_ptr<PQLToken> token2,
                                                                       const std::string &patternStr) {
    return std::make_unique<AssignPatternClause>(std::move(token1), std::move(token2), patternStr);
}

std::unique_ptr<TwoArgClause> ClauseFactory::createClause(std::unique_ptr<PQLToken> token1,
                                                          std::unique_ptr<PQLToken> token2,
                                                          const std::string &relationship) {
    if (relationship == WITHENT_KEYWORD) {
        return std::make_unique<WithEntClause>(std::move(token1), std::move(token2));
    } else if (relationship == WITHNUM_KEYWORD) {
        return std::make_unique<WithNumClause>(std::move(token1), std::move(token2));
    } else if (relationship == MODIFIESS_KEYWORD) {
        return std::make_unique<ModifiesS>(std::move(token1), std::move(token2));
    } else if (relationship == MODIFIESP_KEYWORD) {
        return std::make_unique<ModifiesP>(std::move(token1), std::move(token2));
    } else if (relationship == USESS_KEYWORD) {
        return std::make_unique<UsesS>(std::move(token1), std::move(token2));
    } else if (relationship == USESP_KEYWORD) {
        return std::make_unique<UsesP>(std::move(token1), std::move(token2));
    } else if (relationship == FOLLOW_KEYWORD) {
        return std::make_unique<Follows>(std::move(token1), std::move(token2), false);
    } else if (relationship == FOLLOWSTAR_KEYWORD) {
        return std::make_unique<Follows>(std::move(token1), std::move(token2), true);
    } else if (relationship == PARENT_KEYWORD) {
        return std::make_unique<Parent>(std::move(token1), std::move(token2), false);
    } else if (relationship == PARENTSTAR_KEYWORD) {
        return std::make_unique<Parent>(std::move(token1), std::move(token2), true);
    } else if (relationship == CALLS_KEYWORD) {
        return std::make_unique<Calls>(std::move(token1), std::move(token2), false);
    } else if (relationship == CALLSSTAR_KEYWORD) {
        return std::make_unique<Calls>(std::move(token1), std::move(token2), true);
    } else if (relationship == NEXT_KEYWORD) {
        return std::make_unique<Next>(std::move(token1), std::move(token2), false);
    } else if (relationship == NEXTSTAR_KEYWORD) {
        return std::make_unique<Next>(std::move(token1), std::move(token2), true);
    } else if (relationship == AFFECTS_KEYWORD) {
        return std::make_unique<Affects>(std::move(token1), std::move(token2), false);
    } else if (relationship == AFFECTSSTAR_KEYWORD) {
        return std::make_unique<Affects>(std::move(token1), std::move(token2), true);
    } else {
        throw std::runtime_error("Invalid relationship given to clause factory!");
    }
}

std::unique_ptr<OneArgClause> ClauseFactory::createIfPatternClause(std::unique_ptr<PQLToken> token1,
                                                                   const std::string &patternStr) {
    return std::make_unique<IfPattern>(std::move(token1), patternStr);
}

std::unique_ptr<OneArgClause> ClauseFactory::createWhilePatternClause(std::unique_ptr<PQLToken> token1,
                                                                      const std::string &patternStr) {
    return std::make_unique<WhilePattern>(std::move(token1), patternStr);
}
