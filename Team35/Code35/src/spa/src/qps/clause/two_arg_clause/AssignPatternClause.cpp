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

std::unique_ptr<Result> AssignPatternClause::evaluate(PKBReader *db) {
    /* <var SYNONYM | IDENT | _> , <EXPR | _EXPR_ | _> */
    STMT_SET stmtSet;
    STMT_ENT_SET stmtVarSet;
    bool hasWildcard = false;

    if (second_->tag == PQLToken::Tag::EXPR) {
        ASSIGN_PAT_RIGHT pattern = dynamic_cast<Expression &>(*second_).exprNode;
        hasWildcard = dynamic_cast<Expression &>(*second_).hasWildcard;
        if (!hasWildcard) {  // exact
            stmtSet = db->getStmtWithExactPatternMatch(pattern);
            stmtVarSet = db->getStmtVarExactPatternMatch(pattern);
        } else {  // partial
            stmtSet = db->getStmtWithPartialPatternMatch(pattern);
            stmtVarSet = db->getStmtVarPartialPatternMatch(pattern);
        }
    }
    switch (getPairEnum()) {
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::EXPR):  // a(_, _"x"_) -> int[]
            return std::make_unique<TableResult>(this->ident_, stmtSet);

        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::WILDCARD):  // a(_, _) -> int[]
        {
            STMT_SET assign = db->getStatements(StmtType::Assign);
            return std::make_unique<TableResult>(this->ident_, assign);
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::EXPR):  // a(v, _"x"_) -> pair<int, str>[]
            return std::make_unique<TableResult>(this->ident_, dynamic_cast<Synonym &>(*first_).ident, stmtVarSet);
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // a(v, _) -> pair<int, str>[]
        {
            STMT_ENT_SET modifiesSet = db->getAllRelationshipsWithFilter(StmtNameRelationship::Modifies,
                                                                         StmtType::Assign);
            return std::make_unique<TableResult>(this->ident_, dynamic_cast<Synonym &>(*first_).ident, modifiesSet);
        }
        case pairEnum(PQLToken::Tag::IDENT, PQLToken::Tag::EXPR):  // a("x", "_1_") -> int[]
        {
            ASSIGN_PAT_RIGHT pattern = dynamic_cast<Expression &>(*second_).exprNode;
            STMT_SET result;
            if (hasWildcard) {
                result = db->getStmtWithPartialPatternIntersect(first_->str(), pattern);

            } else {
                result = db->getStmtWithExactPatternIntersect(first_->str(), pattern);
            }
            return std::make_unique<TableResult>(this->ident_, result);
        }
        case pairEnum(PQLToken::Tag::IDENT, PQLToken::Tag::WILDCARD):  // a("x", _) -> int[]
        {
            STMT_SET stmtSet1 = db->getRelationshipWithFilter(StmtNameRelationship::Modifies,
                                                              first_->str(),
                                                              StmtType::Assign);
            return std::make_unique<TableResult>(this->ident_, stmtSet1);
        }
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
