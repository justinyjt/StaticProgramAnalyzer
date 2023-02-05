/*
 * 2/1/2023
 * 1. Token Validator?
 * 2. Error Handling
 * 3. expr / term / factor repeating code
 * 4. CMake
 *
 * 2/4/2023
 * 1. Separate ASTNode or current
 * 2. test() accept() expect() move to another interface
 * */

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Parser.h"
#include "commons/ASTNode.h"
using std::unique_ptr;

Parser::Parser(Lexer &lex, PROGRAM src, DesignExtractor &de) : lex_(lex), src_(std::move(src)), de_(de){}

int Parser::test(const std::string& type) {
    if (lex_.empty()) {
        std::runtime_error("Error: Token List running out!");
    }
    if (lex_.peek().getTokenType() == type) {
        return 1;
    }
    return 0;
}

int Parser::accept(const std::string& type) {
    if (lex_.empty()) {
        std::runtime_error("Error: Token List running out!");
    }
    if (lex_.peek().getTokenType() == type) {
        lex_.move();
        return 1;
    }
    return 0;
}

int Parser::expect(const std::string& type) {
    if (accept(type) == 1) {
        return 1;
    }
    std::runtime_error give_me_a_name("Error: unexpected token");
    return 0;
}

unique_ptr<ASTNode> Parser::Parse() {
    lex_.tokenize(src_);
    unique_ptr<ASTNode> root = std::make_unique<ASTNode>
            (ASTNode::SyntaxType::program,
             std::nullopt);

    while (accept("Procedure")) {
        root->addChild(parseProc());
    }

    expect("EOF");
    return root;
}

unique_ptr<ASTNode> Parser::parseProc() {
    test("Name");
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(
            ASTNode::SyntaxType::procedure,
            lex_.peek().getStringValue()
            );
    lex_.move();
    cur->addChild(parseStmtLst());
    return cur;
}

unique_ptr<ASTNode> Parser::parseStmtLst() {
    expect("{");
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(
            ASTNode::SyntaxType::stmtLst,
            std::nullopt
    );
    while(accept("}") != 1) {
        cur->addChild(parseStmt());
    }
    return cur;
}

unique_ptr<ASTNode> Parser::parseStmt() {
    unique_ptr<ASTNode> cur;
    if (test("Name")) {
        cur = parseAssign();
    } else {
        std::runtime_error("Unidentified Token");
    }
    return cur;
}

unique_ptr<ASTNode> Parser::parseAssign() {
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(
            ASTNode::SyntaxType::assign,
            std::nullopt
    );

    cur->addChild(parseName());
    expect("=");
    cur->addChild(parseExpr());
    expect(";");
    return cur;
}

unique_ptr<ASTNode> Parser::parseExpr() {
    unique_ptr<ASTNode> firstOp = parseTerm();
    if (accept("+")) {
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(
                ASTNode::SyntaxType::plus,
                "+"
        );
        unique_ptr<ASTNode> secondOp = parseExpr();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return op;
    } else if (accept("-")) {
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(
                ASTNode::SyntaxType::minus,
                "-"
        );
        unique_ptr<ASTNode> secondOp = parseExpr();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return op;
    } else {
        return firstOp;
    }
}

unique_ptr<ASTNode> Parser::parseTerm() {
    unique_ptr<ASTNode> firstOp = parseFactor();
    if (accept("*")) {
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(
                ASTNode::SyntaxType::times,
                "*"
        );
        unique_ptr<ASTNode> secondOp = parseTerm();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return op;
    } else if (accept("/")) {
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(
                ASTNode::SyntaxType::div,
                "/"
        );
        unique_ptr<ASTNode> secondOp = parseTerm();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return op;
    } else if (accept("%")) {
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(
                ASTNode::SyntaxType::mod,
                "%"
        );
        unique_ptr<ASTNode> secondOp = parseTerm();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return op;
    } else {
        return firstOp;
    }
}

unique_ptr<ASTNode> Parser::parseFactor() {
    unique_ptr<ASTNode> cur;
    if (test("Name")) {
        cur = parseName();
    } else if (test("Integer")) {
        cur = parseInteger();
    } else if (accept("(")) {
        cur = parseExpr();
        expect(")");
    } else {
        std::runtime_error("Factor: syntax error");
    }
    return cur;
}

unique_ptr<ASTNode> Parser::parseName() {
    test("Name");
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(
            ASTNode::SyntaxType::var,
            lex_.peek().getStringValue()
            );
    lex_.move();
    return cur;
}

unique_ptr<ASTNode> Parser::parseInteger() {
    test("Integer");
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(
            ASTNode::SyntaxType::constVal,
            lex_.peek().getStringValue()
    );
    lex_.move();
    return cur;
}



