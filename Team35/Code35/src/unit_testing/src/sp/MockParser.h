#pragma once

#include <string>
#include <memory>
#include "../src/sp/IParser.h"
#include "commons/ASTNode.h"
#include "commons/token/Token.h"
#include "../src/sp/DesignExtractor.h"
#include "MockLexer.h"


using std::unique_ptr;

class MockParser : public IParser {
public:
    explicit MockParser(PROGRAM src, const DesignExtractor& de, const MockLexer& lex);
    unique_ptr<ASTNode> Parse() override;

private:
    std::unique_ptr<MockLexer> lex_;
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
    unique_ptr<ASTNode> parseExpr();
    unique_ptr<ASTNode> parseTerm();
    unique_ptr<ASTNode> parseFactor();

    unique_ptr<ASTNode> parseName();
    unique_ptr<ASTNode> parseInteger();
};
