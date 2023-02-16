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
    std::deque<std::unique_ptr<Token>> token_lst_;
    uint32_t cur_idx_ = 0;

    void initialise();

    bool peek(Token::Tag tag);
    bool match(Token::Tag tag);
    bool peekOffset(Token::Tag tag, uint32_t offset);
    void next();
    bool isOffsetValid(uint32_t offset);

    bool validateProc();
    bool validateStmtLst();
    bool validateStmt();
    bool validateAssign();
    bool validateRead();
    bool validatePrint();
    bool validateIf();
    bool validateWhile();
    bool validateCondExpr();
    bool validateRelExpr();
    bool validateExpr();
    bool validateTerm();
    bool validateFactor();
    bool validateName();
    bool validateInt();

    bool isName();
    bool isCondExprSeparatedByLogicalOperator();
};


