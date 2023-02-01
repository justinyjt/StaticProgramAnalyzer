#pragma once

#include <string>
#include "IParser.h"
#include "commons/ASTNode.h"
#include "commons/Token.h"
using std::shared_ptr;

class Parser : public IParser {
public:
    explicit Parser();
    shared_ptr<ASTNode> Parse(PROGRAM src) override;

private:
    static int test(const std::string&, std::list<Token>*);
    static int accept(const std::string&, std::list<Token>*);
    static int expect(const std::string&, std::list<Token>*);

    shared_ptr<ASTNode> parseProc(std::list<Token>* tokenLst);
    shared_ptr<ASTNode> parseStmtLst(std::list<Token>* tokenLst);
    shared_ptr<ASTNode> parseStmt(std::list<Token>* tokenLst);
    shared_ptr<ASTNode> parseAssign(std::list<Token>* tokenLst);
    shared_ptr<ASTNode> parseExpr(std::list<Token>* tokenLst);
    shared_ptr<ASTNode> parseTerm(std::list<Token>* tokenLst);
    shared_ptr<ASTNode> parseFactor(std::list<Token>* tokenLst);

    shared_ptr<ASTNode> parseName(std::list<Token>* tokenLst);
    shared_ptr<ASTNode> parseInteger(std::list<Token>* tokenLst);
};
