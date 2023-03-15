#include "TwoArgClauseFactory.h"
#include "qps/clause/TwoArgClause/StmtEntClause.h"
#include "qps/clause/TwoArgClause/EntEntClause.h"
#include "qps/clause/TwoArgClause/StmtStmtClause.h"
#include "qps/clause/TwoArgClause/WithEntClause.h"
#include "qps/clause/TwoArgClause/WithNumClause.h"
#include "qps/clause/TwoArgClause/AssignPattern.h"
#include "qps/query_parser/helper.h"

std::unique_ptr<TwoArgClause> TwoArgClauseFactory::createAssignPatternClause(std::unique_ptr<PQLToken> token1, std::unique_ptr<PQLToken> token2, std::string patternStr) {
    std::unique_ptr<TwoArgClause> a = std::make_unique<AssignPattern>(std::move(token1), std::move(token2), patternStr);
    return std::move(a);
}

std::unique_ptr<TwoArgClause> TwoArgClauseFactory::createClause(std::unique_ptr<PQLToken> token1, std::unique_ptr<PQLToken> token2, std::string relationship) {
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
    } else {}
}
