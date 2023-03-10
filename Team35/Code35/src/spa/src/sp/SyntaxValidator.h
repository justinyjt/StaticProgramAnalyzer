#pragma once

#include <memory>
#include <deque>

#include "commons/lexer/ILexer.h"
#include "commons/token/Token.h"
#include "commons/token_scanner/SimpleTokenScanner.h"

class SyntaxValidator {
 public:
    explicit SyntaxValidator(std::unique_ptr<ILexer> lex);
    bool validateProgram();
    std::deque<std::unique_ptr<Token>> getTokenLst();

 private:
    SimpleTokenScanner scanner_;

    void reset();

    bool validateProc();
    bool validateStmtLst();
    bool validateStmt();

    bool validateAssign();
    bool validateRead();
    bool validatePrint();
    bool validateIf();
    bool validateWhile();
    bool validateCall();

    bool validateCondExpr();
    bool validateRelExpr();
    bool validateExpr();
    bool validateTerm();
    bool validateFactor();
    bool validateName();
    bool validateInt();
};


