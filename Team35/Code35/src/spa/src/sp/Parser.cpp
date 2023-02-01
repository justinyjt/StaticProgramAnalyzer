/*
 * 2/1/2023
 * 1. Token Validator?
 * 2. Error Handling
 * 3. expr / term / factor repeating code
 * 4. CMake
 * */

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Parser.h"
#include "commons/ASTNode.h"
#include "commons/Token.h"
using std::shared_ptr;

Parser::Parser() {}

int Parser::test(const std::string& type, std::list<Token>* tokenLst) {
    if (tokenLst->empty()) {
        std::runtime_error("Error: Token List running out!");
    }
    if (tokenLst->front().getTokenType() == type) {
        return 1;
    }
    return 0;
}

int Parser::accept(const std::string& type, std::list<Token>* tokenLst) {
    if (tokenLst->empty()) {
        std::runtime_error("Error: Token List running out!");
    }
    if (tokenLst->front().getTokenType() == type) {
        tokenLst->pop_front();
        return 1;
    }
    return 0;
}

int Parser::expect(const std::string& type, std::list<Token>* tokenLst) {
    if (accept(type, tokenLst) == 1) {
        return 1;
    }
    std::runtime_error give_me_a_name("Error: unexpected token");
    return 0;
}

shared_ptr<ASTNode> Parser::Parse(PROGRAM src) {
    std::list<Token> tokenLst = {};
    shared_ptr<ASTNode> root = std::make_shared<ASTNode>
            (ASTNode::SyntaxType::program,
             std::nullopt);

    while(accept("Procedure", &tokenLst) == 1) {
        root->addChild(parseProc(&tokenLst));
    }
    expect("EOF", &tokenLst);
    return root;
}

shared_ptr<ASTNode> Parser::parseProc(std::list<Token>* tokenLst) {
    test("Name", tokenLst);
    shared_ptr<ASTNode> cur = std::make_shared<ASTNode>(
            ASTNode::SyntaxType::procedure,
            tokenLst->front().getStringValue()
            );
    tokenLst->pop_front();
    cur->addChild(parseStmtLst(tokenLst));
    return cur;
}

shared_ptr<ASTNode> Parser::parseStmtLst(std::list<Token>* tokenLst) {
    expect("{", tokenLst);
    shared_ptr<ASTNode> cur = std::make_shared<ASTNode>(
            ASTNode::SyntaxType::stmtLst,
            std::nullopt
    );
    while(accept("}", tokenLst) != 1) {
        cur->addChild(parseStmt(tokenLst));
    }
    return cur;
}

shared_ptr<ASTNode> Parser::parseStmt(std::list<Token> *tokenLst) {
    shared_ptr<ASTNode> cur;
    if (test("Name", tokenLst)) {
        cur = parseAssign(tokenLst);
    } else {
        std::runtime_error("Unidentified Token");
    }
    return cur;
}

shared_ptr<ASTNode> Parser::parseAssign(std::list<Token> *tokenLst) {
    shared_ptr<ASTNode> cur = std::make_shared<ASTNode>(
            ASTNode::SyntaxType::assign,
            std::nullopt
    );

    cur->addChild(parseName(tokenLst));
    expect("=", tokenLst);
    cur->addChild(parseExpr(tokenLst));
    expect(";", tokenLst);
    return cur;
}

shared_ptr<ASTNode> Parser::parseExpr(std::list<Token> *tokenLst) {
    shared_ptr<ASTNode> firstOp = parseTerm(tokenLst);
    if (accept("+", tokenLst)) {
        shared_ptr<ASTNode> op = std::make_shared<ASTNode>(
                ASTNode::SyntaxType::plus,
                std::nullopt
        );
        shared_ptr<ASTNode> secondOp = parseExpr(tokenLst);
        op->addChild(firstOp);
        op->addChild(secondOp);

        return op;
    } else if (accept("-", tokenLst)) {
        shared_ptr<ASTNode> op = std::make_shared<ASTNode>(
                ASTNode::SyntaxType::minus,
                std::nullopt
        );
        shared_ptr<ASTNode> secondOp = parseExpr(tokenLst);
        op->addChild(firstOp);
        op->addChild(secondOp);

        return op;
    } else {
        return firstOp;
    }
}

shared_ptr<ASTNode> Parser::parseTerm(std::list<Token> *tokenLst) {
    shared_ptr<ASTNode> firstOp = parseFactor(tokenLst);
    if (accept("*", tokenLst)) {
        shared_ptr<ASTNode> op = std::make_shared<ASTNode>(
                ASTNode::SyntaxType::times,
                std::nullopt
        );
        shared_ptr<ASTNode> secondOp = parseTerm(tokenLst);
        op->addChild(firstOp);
        op->addChild(secondOp);

        return op;
    } else if (accept("/", tokenLst)) {
        shared_ptr<ASTNode> op = std::make_shared<ASTNode>(
                ASTNode::SyntaxType::div,
                std::nullopt
        );
        shared_ptr<ASTNode> secondOp = parseTerm(tokenLst);
        op->addChild(firstOp);
        op->addChild(secondOp);

        return op;
    } else if (accept("%", tokenLst)) {
        shared_ptr<ASTNode> op = std::make_shared<ASTNode>(
                ASTNode::SyntaxType::mod,
                std::nullopt
        );
        shared_ptr<ASTNode> secondOp = parseTerm(tokenLst);
        op->addChild(firstOp);
        op->addChild(secondOp);

        return op;
    } else {
        return firstOp;
    }
}

shared_ptr<ASTNode> Parser::parseFactor(std::list<Token> *tokenLst) {
    shared_ptr<ASTNode> cur;
    if (test("Name", tokenLst)) {
        cur = parseName(tokenLst);
    } else if (test("Integer", tokenLst)) {
        cur = parseInteger(tokenLst);
    } else if (accept("(", tokenLst)) {
        cur = parseExpr(tokenLst);
        expect(")", tokenLst);
    } else {
        std::runtime_error("Factor: syntax error");
    }
    return cur;
}

shared_ptr<ASTNode> Parser::parseName(std::list<Token> *tokenLst) {
    test("Name", tokenLst);
    shared_ptr<ASTNode> cur = std::make_shared<ASTNode>(
            ASTNode::SyntaxType::var,
            tokenLst->front().getStringValue()
            );
    tokenLst->pop_front();
    return cur;
}

shared_ptr<ASTNode> Parser::parseInteger(std::list<Token> *tokenLst) {
    test("Integer", tokenLst);
    shared_ptr<ASTNode> cur = std::make_shared<ASTNode>(
            ASTNode::SyntaxType::constVal,
            tokenLst->front().getStringValue()
    );
    tokenLst->pop_front();
    return cur;
}



