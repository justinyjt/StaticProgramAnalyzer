#pragma once

#include <string>
#include <memory>
#include "IParser.h"
#include "commons/ASTNode.h"
#include "commons/token/Token.h"
#include "DesignExtractor.h"

using std::unique_ptr;

class Parser : public IParser {
 public:
    explicit Parser(PROGRAM src, const DesignExtractor& de);
    unique_ptr<ASTNode> Parse() override;

 private:
    std::unique_ptr<ILexer> lex_;
    PROGRAM src_;
    DesignExtractor &de_;
    Token cur_;
    bool isRead_;

    int peek(Token::Tag);
    int accept(Token::Tag);
    int expect(Token::Tag);

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
