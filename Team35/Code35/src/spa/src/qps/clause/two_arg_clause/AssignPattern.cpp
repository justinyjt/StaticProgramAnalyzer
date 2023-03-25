#include "AssignPattern.h"

#include <list>
#include <vector>

#include "commons/expr_parser/ExprParser.h"
#include "commons/lexer/LexerFactory.h"
#include "commons/token_scanner/TokenScanner.h"

AssignPattern::AssignPattern(std::unique_ptr<PQLToken> first, std::unique_ptr<PQLToken> second,
                             std::string ident) : TwoArgClause(std::move(first), std::move(second)), ident(ident) {
    validateArgs();
}

std::unique_ptr<Result> AssignPattern::evaluate(PKBReader *db) {
    /* <var SYNONYM | IDENT | _> , <EXPR | _EXPR_ | _> */
    STMT_SET stmtSet2;

    if (second->tag == PQLToken::Tag::EXPR) {
        ASSIGN_PAT_RIGHT pattern = dynamic_cast<Expression &>(*second).exprNode;
        bool hasWildcard = dynamic_cast<Expression &>(*second).hasWildcard;
        if (!hasWildcard) {  // exact
            stmtSet2 = db->getStmtWithExactPatternMatch(pattern);
        } else {  // partial
            stmtSet2 = db->getStmtWithPartialPatternMatch(pattern);
        }
    }

    STMT_SET assignSet = db->getStatements(StmtType::Assign);
    std::unique_ptr<Result> filterSet = std::make_unique<TableResult>(this->ident, assignSet);

    switch (getPairEnum()) {
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::EXPR):  // a(_, _"x"_) -> int[]
        {
            std::unique_ptr<Result> result = std::make_unique<TableResult>(this->ident, stmtSet2);
            return std::move(result->join(*filterSet));
        }
        case pairEnum(PQLToken::Tag::WILDCARD, PQLToken::Tag::WILDCARD):  // a(_, _) -> int[]
        {
            STMT_SET stmtSet = db->getStatements(StmtType::Assign);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(this->ident, stmtSet);
            return std::move(result->join(*filterSet));
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::EXPR):  // a(v, _"x"_) -> pair<int, str>[]
        {
            const std::string &synonymIdent = dynamic_cast<Synonym &>(*first).ident;
            std::vector<std::vector<std::string>> vec;
            for (STMT_NUM s : stmtSet2) {  // for each statement, find entity that is modified
                ENT_SET entSet = db->getRelationship(StmtNameRelationship::Modifies, s);
                for (const std::string &ent : entSet)
                    vec.push_back({std::to_string(s), ent});
            }
            std::unique_ptr<Result> result = std::make_unique<TableResult>(this->ident, synonymIdent, vec);
            return std::move(result->join(*filterSet));
        }
        case pairEnum(PQLToken::Tag::SYNONYM, PQLToken::Tag::WILDCARD):  // a(v, _) -> pair<int, str>[]
        {
            const std::string &synonymIdent = dynamic_cast<Synonym &>(*first).ident;
            STMT_ENT_SET stmtEntSet = db->getAllRelationships(StmtNameRelationship::Modifies);
            std::unique_ptr<Result> result = std::make_unique<TableResult>(this->ident, synonymIdent, stmtEntSet);
            return std::move(result->join(*filterSet));
        }
        case pairEnum(PQLToken::Tag::IDENT, PQLToken::Tag::EXPR):  // a("x", "_1_") -> int[]
        {
            STMT_SET stmtSet1 = db->getRelationship(StmtNameRelationship::Modifies, first->str());
            STMT_SET stmtSetResult;
            for (STMT_NUM s : stmtSet1) {  // find s1 n s2
                if (stmtSet2.find(s) != stmtSet2.end()) {
                    stmtSetResult.insert(s);
                }
            }
            std::unique_ptr<Result> result = std::make_unique<TableResult>(this->ident, stmtSetResult);
            return std::move(result->join(*filterSet));
        }
        case pairEnum(PQLToken::Tag::IDENT, PQLToken::Tag::WILDCARD):  // a("x", _) -> int[]
        {
            STMT_SET stmtSet1 = db->getRelationship(StmtNameRelationship::Modifies, first->str());
            std::unique_ptr<Result> result = std::make_unique<TableResult>(this->ident, stmtSet1);
            return std::move(result->join(*filterSet));
        }
        default:throw std::runtime_error("AssignPattern.cpp");
    }
}

void AssignPattern::validateArgs() {
    const Synonym *synonym1 = dynamic_cast<const Synonym *>(first.get());
    if (synonym1 != nullptr && synonym1->de != Synonym::DesignEntity::VARIABLE) {
        throw SemanticException();
    }
}

bool AssignPattern::operator==(const Clause &rhs) const {
    const auto *pRhs = dynamic_cast<const AssignPattern *>(&rhs);
    return pRhs != nullptr && equal(*pRhs) && ident == pRhs->ident;
}
