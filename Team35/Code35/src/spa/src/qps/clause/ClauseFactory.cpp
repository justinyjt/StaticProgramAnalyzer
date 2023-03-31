#include "ClauseFactory.h"
#include "qps/clause/two_arg_clause/StmtEntClause.h"
#include "qps/clause/two_arg_clause/EntEntClause.h"
#include "qps/clause/two_arg_clause/StmtStmtClause.h"
#include "qps/clause/two_arg_clause/WithEntClause.h"
#include "qps/clause/two_arg_clause/WithNumClause.h"
#include "qps/clause/two_arg_clause/AssignPattern.h"
#include "qps/query_parser/helper.h"

std::unique_ptr<TwoArgClause> ClauseFactory::createAssignPatternClause(std::unique_ptr<PQLToken> token1,
                                                                       std::unique_ptr<PQLToken> token2,
                                                                       const std::string &patternStr) {
    std::unique_ptr<TwoArgClause> a = std::make_unique<AssignPattern>(std::move(token1), std::move(token2), patternStr);
    return std::move(a);
}

std::unique_ptr<TwoArgClause> ClauseFactory::createClause(std::unique_ptr<PQLToken> token1,
                                                          std::unique_ptr<PQLToken> token2,
                                                          const std::string &relationship) {
    if (relationship == WITHENT_KEYWORD) {
        std::unique_ptr<WithEntClause> w = std::make_unique<WithEntClause>(std::move(token1), std::move(token2));
        return std::move(w);
    } else if (relationship == WITHNUM_KEYWORD) {
        std::unique_ptr<WithNumClause> w = std::make_unique<WithNumClause>(std::move(token1), std::move(token2));
        return std::move(w);
    } else if (relationship == MODIFIESS_KEYWORD) {
        std::unique_ptr<ModifiesS> m = std::make_unique<ModifiesS>(std::move(token1), std::move(token2));
        return std::move(m);
    } else if (relationship == MODIFIESP_KEYWORD) {
        std::unique_ptr<ModifiesP> m = std::make_unique<ModifiesP>(std::move(token1), std::move(token2));
        return std::move(m);
    } else if (relationship == USESS_KEYWORD) {
        std::unique_ptr<UsesS> u = std::make_unique<UsesS>(std::move(token1), std::move(token2));
        return std::move(u);
    } else if (relationship == USESP_KEYWORD) {
        std::unique_ptr<UsesP> u = std::make_unique<UsesP>(std::move(token1), std::move(token2));
        return std::move(u);
    } else if (relationship == FOLLOW_KEYWORD) {
        std::unique_ptr<Follows> f = std::make_unique<Follows>(std::move(token1), std::move(token2), false);
        return std::move(f);
    } else if (relationship == FOLLOWSTAR_KEYWORD) {
        std::unique_ptr<Follows> f = std::make_unique<Follows>(std::move(token1), std::move(token2), true);
        return std::move(f);
    } else if (relationship == PARENT_KEYWORD) {
        std::unique_ptr<Parent> p = std::make_unique<Parent>(std::move(token1), std::move(token2), false);
        return std::move(p);
    } else if (relationship == PARENTSTAR_KEYWORD) {
        std::unique_ptr<Parent> p = std::make_unique<Parent>(std::move(token1), std::move(token2), true);
        return std::move(p);
    } else if (relationship == CALLS_KEYWORD) {
        std::unique_ptr<Calls> p = std::make_unique<Calls>(std::move(token1), std::move(token2), false);
        return std::move(p);
    } else if (relationship == CALLSSTAR_KEYWORD) {
        std::unique_ptr<Calls> p = std::make_unique<Calls>(std::move(token1), std::move(token2), true);
        return std::move(p);
    } else if (relationship == NEXT_KEYWORD) {
        std::unique_ptr<Next> n = std::make_unique<Next>(std::move(token1), std::move(token2), false);
        return std::move(n);
    } else if (relationship == NEXTSTAR_KEYWORD) {
        std::unique_ptr<Next> n = std::make_unique<Next>(std::move(token1), std::move(token2), true);
        return std::move(n);
    } else if (relationship == AFFECTS_KEYWORD) {
        std::unique_ptr<Affects> n = std::make_unique<Affects>(std::move(token1), std::move(token2), false);
        return std::move(n);
    } else if (relationship == AFFECTSSTAR_KEYWORD) {
        std::unique_ptr<Affects> n = std::make_unique<Affects>(std::move(token1), std::move(token2), true);
        return std::move(n);
    }
}

std::unique_ptr<OneArgClause> ClauseFactory::createIfPatternClause(std::unique_ptr<PQLToken> token1,
                                                                   const std::string &patternStr) {
    std::unique_ptr<OneArgClause> a = std::make_unique<IfPattern>(std::move(token1), patternStr);
    return std::move(a);
}

std::unique_ptr<OneArgClause> ClauseFactory::createWhilePatternClause(std::unique_ptr<PQLToken> token1,
                                                                      const std::string &patternStr) {
    std::unique_ptr<OneArgClause> a = std::make_unique<WhilePattern>(std::move(token1), patternStr);
    return std::move(a);
}
