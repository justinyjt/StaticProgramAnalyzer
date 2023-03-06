#include "Modifies.h"

ModifiesS::ModifiesS(std::unique_ptr<PQLToken> first,
                     std::unique_ptr<PQLToken> second) : TwoArgClause(std::move(first), std::move(second)) {
    validateArgs();
}

std::unique_ptr<Result> ModifiesS::evaluate(PKBReader* db) {
    /* <stmt SYNONYM | STMT_NUM>, <var SYNONYM | IDENT | _ > */

    switch (getPairEnum()) {
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::SYNONYM):  // Modifies(1, var) -> string[]
        {
            int stmtNum = dynamic_cast<const StatementNumber*>(first.get())->n;
            ENT_SET set = db->getRelationship(StmtNameRelationship::Modifies, stmtNum);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(second->str(), set);
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::WILDCARD):  // Modifies(1, _) -> bool
        {
            int stmtNum = dynamic_cast<const StatementNumber*>(first.get())->n;
            ENT_SET set = db->getRelationship(StmtNameRelationship::Modifies, stmtNum);
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(!set.empty());
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::STMT_NUM, PQLToken::Tag::IDENT):  // Modifies(1, "x") -> bool
        {
            int stmtNum = dynamic_cast<const StatementNumber*>(first.get())->n;
            bool b = db->isRelationshipExists(StmtNameRelationship::Modifies,
                                            stmtNum, second->str());
            std::unique_ptr<Result> result = std::make_unique<BoolResult>(b);
            return std::move(result);
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::SYNONYM):  // Modifies(stmt, var) -> pair<int, string>[]
        {
            STMT_ENT_SET modifiesSet = db->getAllRelationships(StmtNameRelationship::Modifies);
            STMT_SET filterSet = db->getStatements(getStmtType(dynamic_cast<const Synonym*>(first.get())->de));
            std::unique_ptr<Result> intermediateResult = std::make_unique<TableResult>(first->str(), filterSet);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), second->str(), modifiesSet);
            return std::move(Result::join(result.get(), intermediateResult.get()));
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // Modifies(stmt, _) -> int[]
        {
            STMT_SET modifiesStmtSet = db->getStmtByRelationship(StmtNameRelationship::Modifies);
            STMT_SET filterSet = db->getStatements(getStmtType(dynamic_cast<const Synonym*>(first.get())->de));
            std::unique_ptr<Result> intermediateResult = std::make_unique<TableResult>(first->str(), filterSet);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), modifiesStmtSet);
            return std::move(Result::join(result.get(), intermediateResult.get()));
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::IDENT):  // Modifies(stmt, "x")/ -> int[]
        {
            STMT_SET stmtSet = db->getRelationship(StmtNameRelationship::Modifies, second->str());
            STMT_SET filterSet = db->getStatements(getStmtType(dynamic_cast<const Synonym*>(first.get())->de));
            std::unique_ptr<Result> intermediateResult = std::make_unique<TableResult>(first->str(), filterSet);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(first->str(), stmtSet);
            return std::move(Result::join(result.get(), intermediateResult.get()));
        }
        default:
            throw std::runtime_error("");
    }}

void ModifiesS::validateArgs() {
    const Synonym* synonym1 = dynamic_cast<const Synonym*>(first.get());
    const Synonym* synonym2 = dynamic_cast<const Synonym*>(second.get());
    if (synonym1 != nullptr && (synonym1->de == Synonym::DesignEntity::PRINT) ||
        first->tag == PQLToken::Tag::WILDCARD ||
        (synonym2 != nullptr && synonym2->de != Synonym::DesignEntity::VARIABLE)) {
        throw SemanticException();
    }
}

bool ModifiesS::operator==(const Clause& rhs) const {
    const auto* pRhs = dynamic_cast<const ModifiesS*>(&rhs);
    return pRhs != nullptr && equal(*pRhs);
}
