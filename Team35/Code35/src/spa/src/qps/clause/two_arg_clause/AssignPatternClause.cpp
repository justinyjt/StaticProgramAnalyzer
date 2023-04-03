#include "AssignPatternClause.h"

#include <list>
#include <utility>
#include <vector>

#include "commons/token_scanner/TokenScanner.h"

AssignPatternClause::AssignPatternClause(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second,
                                         std::string ident) : TwoArgClause(std::move(first), std::move(second)),
                                                              ident_(std::move(ident)) {
    this->addHeader(ident_);
    validateArgs();
}

void AssignPatternClause::updatePatternInfo() {
    if (second_->tag == PQLToken::Tag::EXPR) {
        pattern_ = dynamic_cast<Expression &>(*second_).exprNode;
        hasWildcard_ = dynamic_cast<Expression &>(*second_).hasWildcard;
    }
}

STMT_SET AssignPatternClause::getStmtSet(PKBReader *db) {
    if (second_->tag == PQLToken::Tag::EXPR) {
        if (hasWildcard_) {
            return db->getStmtWithPartialPatternMatch(pattern_);
        } else {
            return db->getStmtWithExactPatternMatch(pattern_);
        }
    }
    return {};
}

STMT_ENT_SET AssignPatternClause::getStmtVarSet(PKBReader *db) {
    if (second_->tag == PQLToken::Tag::EXPR) {
        if (hasWildcard_) {
            return db->getStmtVarPartialPatternMatch(pattern_);;
        } else {
            return db->getStmtVarExactPatternMatch(pattern_);;
        }
    }
    return {};
}

std::unique_ptr<Result> AssignPatternClause::handleIdentExpr(PKBReader *db) {
    STMT_SET result;
    if (hasWildcard_) {
        result = db->getStmtWithPartialPatternIntersect(first_->str(), pattern_);
    } else {
        result = db->getStmtWithExactPatternIntersect(first_->str(), pattern_);
    }
    return std::make_unique<TableResult>(this->ident_, result);
}

std::unique_ptr<Result> AssignPatternClause::handleWildCardWildCard(PKBReader *db) {
    STMT_SET assign = db->getStatements(StmtType::Assign);
    return std::make_unique<TableResult>(this->ident_, assign);
}

std::unique_ptr<Result> AssignPatternClause::handleWildCardExpr(PKBReader *db) {
    STMT_SET result = this->getStmtSet(db);
    return std::make_unique<TableResult>(this->ident_, result);
}

std::unique_ptr<Result> AssignPatternClause::handleSynExpr(PKBReader *db) {
    STMT_ENT_SET result = this->getStmtVarSet(db);
    return std::make_unique<TableResult>(this->ident_, dynamic_cast<Synonym &>(*first_).ident, result);
}

std::unique_ptr<Result> AssignPatternClause::handleSynWildCard(PKBReader *db) {
    STMT_ENT_SET modifiesSet = db->getAllRelationshipsWithFilter(StmtNameRelationship::Modifies,
                                                                 StmtType::Assign);
    return std::make_unique<TableResult>(this->ident_, dynamic_cast<Synonym &>(*first_).ident, modifiesSet);
}

std::unique_ptr<Result> AssignPatternClause::handleIdentWildCard(PKBReader *db) {
    STMT_SET result = db->getRelationshipWithFilter(StmtNameRelationship::Modifies,
                                                    first_->str(),
                                                    StmtType::Assign);
    return std::make_unique<TableResult>(this->ident_, result);
}

std::unique_ptr<Result> AssignPatternClause::evaluate(PKBReader *db) {
    /* <var SYNONYM | IDENT | _> , <EXPR | _EXPR_ | _> */
    updatePatternInfo();
    switch (getPairEnum()) {
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::EXPR):  // a(_, _"x"_) -> int[]
            return handleWildCardExpr(db);
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::WILDCARD):  // a(_, _) -> int[]
            return handleWildCardWildCard(db);
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::EXPR):  // a(v, _"x"_) -> pair<int, str>[]
            return handleSynExpr(db);
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // a(v, _) -> pair<int, str>[]
            return handleSynWildCard(db);
        case pairEnum(PQLToken::Tag::IDENT, PQLToken::Tag::EXPR):  // a("x", "_1_") -> int[]
            return handleIdentExpr(db);
        case pairEnum(PQLToken::Tag::IDENT, PQLToken::Tag::WILDCARD):  // a("x", _) -> int[]
            return handleIdentWildCard(db);
        default:
            throw std::runtime_error("AssignPatternClause.cpp");
    }
}

void AssignPatternClause::validateArgs() {
    const Synonym *synonym1 = dynamic_cast<const Synonym *>(first_.get());
    if (synonym1 != nullptr && synonym1->de != Synonym::DesignEntity::VARIABLE) {
        throw SemanticException();
    }
}

bool AssignPatternClause::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const AssignPatternClause *>(&rhs);
    return pRhs != nullptr && equal(*pRhs) && ident_ == pRhs->ident_;
}
