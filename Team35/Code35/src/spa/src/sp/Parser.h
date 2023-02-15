#pragma once

#include <string>
#include <memory>
#include <deque>

#include "IParser.h"
#include "commons/ASTNode.h"
#include "commons/token/Token.h"
#include "DesignExtractor.h"

using std::unique_ptr;

class Parser : public IParser {
 public:
    explicit Parser(
        PROGRAM src, std::unique_ptr<PKBWriter> pkb,
        std::deque<std::unique_ptr<Token>> tokenLst);
    explicit Parser(
        PROGRAM src, std::unique_ptr<PKBWriter> pkb,
        std::deque<std::unique_ptr<Token>> tokenLst, bool flagExtract);
    unique_ptr<ASTNode> Parse() override;

 private:
    std::deque<std::unique_ptr<Token>> tokenLst_;
    PROGRAM src_;
    std::unique_ptr<DesignExtractor> de_;
    bool flagExtract_;

    int peek(Token::Tag);
    Lexeme peekLexeme();
    int accept(Token::Tag);

    unique_ptr<ASTNode> parseProc();
    unique_ptr<ASTNode> parseStmtLst();
    unique_ptr<ASTNode> parseStmt();
    unique_ptr<ASTNode> parseAssign();
    unique_ptr<ASTNode> parseRead();
    unique_ptr<ASTNode> parsePrint();
    unique_ptr<ASTNode> parseExpr();
    unique_ptr<ASTNode> parseTerm();
    unique_ptr<ASTNode> parseFactor();

    unique_ptr<ASTNode> parseName();
    unique_ptr<ASTNode> parseInteger();
};
