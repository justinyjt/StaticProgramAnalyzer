#pragma once

#include <string>
#include "IParser.h"
#include "commons/ASTNode.h"
#include "commons/Token.h"
#include "DesignExtractor.h"

using std::unique_ptr;

class Parser : public IParser {
public:
    explicit Parser(Lexer& lex, PROGRAM src, DesignExtractor& de);
    unique_ptr<ASTNode> Parse() override;

private:
    Lexer &lex_;
    PROGRAM src_;
    DesignExtractor &de_;

    int test(const std::string&);
    int accept(const std::string&);
    int expect(const std::string&);

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
