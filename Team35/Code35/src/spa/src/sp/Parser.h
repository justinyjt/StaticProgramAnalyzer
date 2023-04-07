#pragma once

#include <string>
#include <memory>
#include <deque>

#include "IParser.h"
#include "commons/ASTNode.h"
#include "commons/token/Token.h"
#include "commons/token_scanner/SimpleTokenScanner.h"

using std::shared_ptr;

class Parser : public IParser {
 public:
    explicit Parser(SimpleTokenScanner &scanner);

    shared_ptr<ASTNode> Parse() override;

 private:
    SimpleTokenScanner &scanner_;

    shared_ptr<ASTNode> parseProc();

    shared_ptr<ASTNode> parseStmtLst();

    shared_ptr<ASTNode> parseStmt();

    shared_ptr<ASTNode> parseAssign();

    shared_ptr<ASTNode> parseRead();

    shared_ptr<ASTNode> parsePrint();

    shared_ptr<ASTNode> parseIf();

    shared_ptr<ASTNode> parseWhile();

    shared_ptr<ASTNode> parseCondExpr();

    shared_ptr<ASTNode> parseRelExpr();

    shared_ptr<ASTNode> parseRelFactor();

    shared_ptr<ASTNode> parseExpr();

    shared_ptr<ASTNode> parseName();

    shared_ptr<ASTNode> parseCall();

    shared_ptr<ASTNode> parseTerm();

    shared_ptr<ASTNode> parseFactor();

    shared_ptr<ASTNode> parseInteger();
};
