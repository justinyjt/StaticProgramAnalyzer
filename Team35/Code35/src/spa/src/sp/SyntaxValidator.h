#pragma once

#include <memory>
#include <deque>

#include "commons/token/Token.h"
#include "commons/lexer/ILexer.h"

class SyntaxValidator {
 public:
    explicit SyntaxValidator(std::unique_ptr<ILexer> lex);
    bool validateProgram();
    std::deque<std::unique_ptr<Token>> getTokenLst();

 private:
    std::unique_ptr<ILexer> lex_;
    std::unique_ptr<Token> cur_;
    std::deque<std::unique_ptr<Token>> tokenLst_;

    int peek(Token::Tag);
    int match(Token::Tag);
    void next();

    bool validateProc();
    bool validateStmtLst();
    bool validateStmt();
    bool validateAssign();
    bool validateRead();
    bool validatePrint();
    bool validateExpr();
    bool validateTerm();
    bool validateFactor();
    bool validateName();
    bool validateInt();
};


