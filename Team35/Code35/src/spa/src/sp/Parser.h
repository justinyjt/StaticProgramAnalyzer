#pragma once

#include <string>
#include <memory>
#include <deque>

#include "IParser.h"
#include "commons/ASTNode.h"
#include "commons/token/Token.h"
#include "commons/token_scanner/SimpleTokenScanner.h"

using std::unique_ptr;

class Parser : public IParser {
 public:
    explicit Parser(TokenLst token_lst);
    unique_ptr<ASTNode> Parse() override;

 private:
    SimpleTokenScanner scanner_;

    unique_ptr<ASTNode> parseProc();
    unique_ptr<ASTNode> parseStmtLst();
    unique_ptr<ASTNode> parseStmt();

    unique_ptr<ASTNode> parseAssign();
    unique_ptr<ASTNode> parseRead();
    unique_ptr<ASTNode> parsePrint();
    unique_ptr<ASTNode> parseIf();
    unique_ptr<ASTNode> parseWhile();
    
    unique_ptr<ASTNode> parseCondExpr();
    unique_ptr<ASTNode> parseRelExpr();
    unique_ptr<ASTNode> parseRelFactor();

    unique_ptr<ASTNode> parseExpr();
    unique_ptr<ASTNode> parseTerm();
    unique_ptr<ASTNode> parseFactor();

    unique_ptr<ASTNode> parseName();
    unique_ptr<ASTNode> parseInteger();
};
