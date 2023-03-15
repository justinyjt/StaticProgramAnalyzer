#pragma once

#include <memory>
#include <deque>

#include "commons/lexer/ILexer.h"
#include "commons/token/Token.h"
#include "commons/token_scanner/PQLTokenScanner.h"

class QuerySyntaxValidator {
public:
    explicit QuerySyntaxValidator(std::unique_ptr<ILexer> lex);
    bool validateQuery();
    std::deque<std::unique_ptr<Token>> getTokenLst();

private:
    PQLTokenScanner scanner_;

    // void reset();

    bool validateDeclarationLst();
    bool validateDeclaration();
    bool validateResultClause();
    bool validateClauseLst();
    bool validateClause();
    bool validateSuchThatClause();
    bool validatePatternClause();
    bool validateWithClause();
    bool validateRelCond();
    bool validateRelRef();
    bool validateStmtStmtRef();
    bool validateModifiesUsesRef();
    bool validatePatternCond();
    bool validatePattern();
    bool validateAssignPattern();
    bool validateWhilePattern();
    bool validateIfPattern();
    bool validateAttrCond();
    bool validateAttrCompare();
    bool validateRef();
    bool validateAttrRef();
    bool validateAttrName();
    bool validateEntEntRef();
    bool validateStmtRef();
    bool validateEntRef();
    bool validateExpressionSpec();
    bool validateTuple();
    bool validateElem();
};