#include "Uses.h"

UsesS::UsesS(std::unique_ptr<PQLToken> first,
             std::unique_ptr<PQLToken> second) : TwoArgClause(std::move(first), std::move(second)) {
    validateArgs();
}

std::unique_ptr<Result> UsesS::evaluate(PKBReader* db) {
    /* <stmt SYNONYM | STMT_NUM>, <var SYNONYM | IDENT | _ > */
    switch (getPairEnum()) {
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // Uses(1, var) -> string[]
        {
            int stmtNum = dynamic_cast<const StatementNumber*>(first.get())->n;
            ENT_SET set = db->getRelationship(StmtNameRelationship::Uses, stmtNum);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(second->str(), set);
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::WILDCARD):  // Uses(1, _)/ -> bool
        {
            int stmtNum = dynamic_cast<const StatementNumber*>(first.get())->n;
            ENT_SET set = db->getRelationship(StmtNameRelationship::Uses, stmtNum);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(!set.empty());
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::IDENT):  // Uses(1, "x")/ -> bool
        {
            int stmtNum = dynamic_cast<const StatementNumber*>(first.get())->n;
            bool b = db->isRelationshipExists(StmtNameRelationship::Uses, stmtNum,
                                            second->str());
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(b);
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // Uses(stmt, var)/ -> pair<int, string>[]
        {
            STMT_ENT_SET set = db->getAllRelationships(StmtNameRelationship::Uses);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), second->str(), set);
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Uses(stmt, _)/ -> int[]
        {
            STMT_ENT_SET set = db->getAllRelationships(StmtNameRelationship::Uses);
            ENT_SET eset;
            for (auto& p : set)
                eset.insert(std::to_string(p.first));
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), eset);
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::IDENT):  // Uses(stmt, "x")/ -> int[]
        {
            STMT_SET stmtSet = db->getRelationship(StmtNameRelationship::Uses, second->str());
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), stmtSet);
            return std::move(result);
        }
        default:
            throw std::runtime_error("");
    }
}

void UsesS::validateArgs() {
    const Synonym* synonym1 = dynamic_cast<const Synonym*>(first.get());
    const Synonym* synonym2 = dynamic_cast<const Synonym*>(second.get());
    if (synonym1 != nullptr && (synonym1->de == Synonym::DesignEntity::READ) ||
            first->tag == PQLToken::Tag::WILDCARD ||
        (synonym2 != nullptr && synonym2->de != Synonym::DesignEntity::VARIABLE)) {
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
