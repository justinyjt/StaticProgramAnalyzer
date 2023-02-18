#include "Modifies.h"
#include "qps/pql/StatementNumber.h"
#include "qps/query_exceptions/SemanticException.h"
#include "qps/pql/Wildcard.h"

ModifiesS::ModifiesS(std::shared_ptr<PQLToken> first, std::shared_ptr<PQLToken> second) : TwoArgClause(first, second) {
    validateArgs(first, second);
}

std::unique_ptr<Result> ModifiesS::evaluate(PKBReader *db) {
    /* <stmt SYNONYM | STMT_NUM>, <var SYNONYM | IDENT | _ > */

    switch (caseValue()) {
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // Modifies(1, var) -> string[]
        {
            int stmtNum = std::dynamic_pointer_cast<StatementNumber>(first)->n;
            ENT_SET set = db->getRelationship(StmtNameRelationship::Modifies, stmtNum);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(second->str(), set);
            return std::move(result);
        }
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::WILDCARD):  // Modifies(1, _) -> bool
        {
            int stmtNum = std::dynamic_pointer_cast<StatementNumber>(first)->n;
            ENT_SET set = db->getRelationship(StmtNameRelationship::Modifies, stmtNum);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(set.size() > 0);
            return std::move(result);
        }
        case c(PQLToken::Tag::STMT_NUM, PQLToken::Tag::IDENT):  // Modifies(1, "x") -> bool
        {
            int stmtNum = std::dynamic_pointer_cast<StatementNumber>(first)->n;
            bool b = db->isRelationshipExists(StmtNameRelationship::Modifies,
                                            stmtNum, second->str());
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(b);
            return std::move(result);
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // Modifies(stmt, var) -> pair<int, string>[]
        {
            STMT_ENT_SET set = db->getAllRelationships(StmtNameRelationship::Modifies);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), second->str(), set);
            return std::move(result);
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Modifies(stmt, _) -> int[]
        {
            STMT_ENT_SET set = db->getAllRelationships(StmtNameRelationship::Modifies);
            ENT_SET eset;
            for (auto& p : set)
                eset.insert(std::to_string(p.first));
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), eset);
            return std::move(result);
        }
        case c(PQLToken::Tag::SYNONYM, PQLToken::Tag::IDENT):  // Modifies(stmt, "x")/ -> int[]
        {
            STMT_SET stmtSet = db->getRelationship(StmtNameRelationship::Modifies, second->str());
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), stmtSet);
            return std::move(result);
        }
        default:
            throw std::runtime_error("");
    }}

void ModifiesS::validateArgs(std::shared_ptr<PQLToken> first, std::shared_ptr<PQLToken> second) {
    std::shared_ptr<Wildcard> wildcard = std::dynamic_pointer_cast<Wildcard>(first);
    std::shared_ptr<Synonym> synonym1 = std::dynamic_pointer_cast<Synonym>(first);
    std::shared_ptr<Synonym> synonym2 = std::dynamic_pointer_cast<Synonym>(second);
    if (synonym1 != NULL && (synonym1->de == Synonym::DesignEntity::PRINT) ||
        wildcard != NULL ||
        (synonym2 != NULL && synonym2->de != Synonym::DesignEntity::VARIABLE)) {
        throw SemanticException();
    }
}

bool ModifiesS::operator==(const Clause& rhs) const {
    const ModifiesS* pRhs = dynamic_cast<const ModifiesS*>(&rhs);
    if (pRhs != nullptr) {
        return equal(*pRhs);
    }
    return false;
}
