#include "Uses.h"
#include "qps/pql/StatementNumber.h"
#include "qps/pql/Ident.h"
#include "qps/pql/Wildcard.h"
#include "qps/query_exceptions/SemanticException.h"

UsesS::UsesS(std::shared_ptr<PQLToken> first, std::shared_ptr<PQLToken> second) : TwoArgClause(first, second) {
    validateArgs(first, second);
}

std::unique_ptr<Result> UsesS::evaluate(PKBReader *db) {
    /* <stmt SYNONYM | STMT_NUM>, <var SYNONYM | IDENT | _ > */
    switch (caseValue()) {
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // Uses(1, var) -> string[]
        {
            int stmtNum = std::dynamic_pointer_cast<StatementNumber>(first)->n;
            ENT_SET set = db->getRelationship(StmtNameRelationship::Uses, stmtNum);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(second->str(), set);
            return std::move(result);
        }
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::WILDCARD):  // Uses(1, _)/ -> bool
        {
            int stmtNum = std::dynamic_pointer_cast<StatementNumber>(first)->n;
            ENT_SET set = db->getRelationship(StmtNameRelationship::Uses, stmtNum);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(set.size() > 0);
            return std::move(result);
        }
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::IDENT):  // Uses(1, "x")/ -> bool
        {
            int stmtNum = std::dynamic_pointer_cast<StatementNumber>(first)->n;
            bool b = db->isRelationshipExists(StmtNameRelationship::Uses, stmtNum,
                                            second->str());
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(b);
            return std::move(result);
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // Uses(stmt, var)/ -> pair<int, string>[]
        {
            STMT_ENT_SET set = db->getAllRelationships(StmtNameRelationship::Uses);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), second->str(), set);
            return std::move(result);
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Uses(stmt, _)/ -> int[]
        {
            STMT_ENT_SET set = db->getAllRelationships(StmtNameRelationship::Uses);
            ENT_SET eset;
            for (auto& p : set)
                eset.insert(std::to_string(p.first));
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), eset);
            return std::move(result);
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::IDENT):  // Uses(stmt, "x")/ -> int[]
        {
            STMT_SET stmtSet = db->getRelationship(StmtNameRelationship::Uses, second->str());
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), stmtSet);
            return std::move(result);
        }
        default:
            throw std::runtime_error("");
    }
}

void UsesS::validateArgs(std::shared_ptr<PQLToken> first, std::shared_ptr<PQLToken> second) {
    std::shared_ptr<Wildcard> wildcard = std::dynamic_pointer_cast<Wildcard>(first);
    std::shared_ptr<Synonym> synonym1 = std::dynamic_pointer_cast<Synonym>(first);
    std::shared_ptr<Synonym> synonym2 = std::dynamic_pointer_cast<Synonym>(second);
    if (synonym1 != NULL && (synonym1->de == Synonym::DesignEntity::READ) ||
        wildcard != NULL ||
        (synonym2 != NULL && synonym2->de != Synonym::DesignEntity::VARIABLE)) {
        throw SemanticException();
    }
}

bool UsesS::operator==(const Clause& rhs) const {
    const UsesS* pRhs = dynamic_cast<const UsesS*>(&rhs);
    if (pRhs != nullptr) {
        return equal(*pRhs);
    }
    return false;
}
