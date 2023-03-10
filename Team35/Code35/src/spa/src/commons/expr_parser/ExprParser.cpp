#include "ExprParser.h"

#include <cassert>
#include <memory>
#include <utility>

using std::unique_ptr;

ExprParser::ExprParser(TokenScanner &scanner) : scanner_(scanner) {}

ASSIGN_PAT_RIGHT ExprParser::parseExpr() {
    ASSIGN_PAT_RIGHT firstOp = parseTerm();
    if (scanner_.match(Token::Tag::Plus)) {
        ASSIGN_PAT_RIGHT op = std::make_shared<ExprNode>(ASTNode::SyntaxType::Plus, "+");
        ASSIGN_PAT_RIGHT secondOp = parseExpr();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return std::move(op);
    } else if (scanner_.match(Token::Tag::Minus)) {
        ASSIGN_PAT_RIGHT op = std::make_unique<ExprNode>(ASTNode::SyntaxType::Minus, "-");
        ASSIGN_PAT_RIGHT secondOp = parseExpr();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return std::move(op);
    } else {
        return std::move(firstOp);
    }
}

ASSIGN_PAT_RIGHT ExprParser::parseTerm() {
    ASSIGN_PAT_RIGHT firstOp = parseFactor();
    if (scanner_.match(Token::Tag::Multiply)) {
        ASSIGN_PAT_RIGHT op = std::make_shared<ExprNode>(ASTNode::SyntaxType::Multiply, "*");
        ASSIGN_PAT_RIGHT secondOp = parseTerm();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return std::move(op);
    } else if (scanner_.match(Token::Tag::Divide)) {
        ASSIGN_PAT_RIGHT op = std::make_shared<ExprNode>(ASTNode::SyntaxType::Divide, "/");
        ASSIGN_PAT_RIGHT secondOp = parseTerm();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return std::move(op);
    } else if (scanner_.match(Token::Tag::Modulo)) {
        ASSIGN_PAT_RIGHT op = std::make_shared<ExprNode>(ASTNode::SyntaxType::Modulo, "%");
        ASSIGN_PAT_RIGHT secondOp = parseTerm();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return std::move(op);
    } else {
        return std::move(firstOp);
    }
}

ASSIGN_PAT_RIGHT ExprParser::parseFactor() {
    ASSIGN_PAT_RIGHT cur;
    if (scanner_.peek(Token::Tag::Name)) {
        cur = parseName();
    } else if (scanner_.peek(Token::Tag::Integer)) {
        cur = parseInteger();
    } else {
        scanner_.match(Token::Tag::LParen);
        cur = parseExpr();
        scanner_.match(Token::Tag::RParen);
    }
    return std::move(cur);
}

ASSIGN_PAT_RIGHT ExprParser::parseName() {
    assert(scanner_.peek(Token::Tag::Name));
    ASSIGN_PAT_RIGHT cur =
            std::make_shared<ExprNode>(ASTNode::SyntaxType::Variable, scanner_.peekLexeme());
    scanner_.match(Token::Tag::Name);
    return std::move(cur);
}

ASSIGN_PAT_RIGHT ExprParser::parseInteger() {
    assert(scanner_.peek(Token::Tag::Integer));
    ASSIGN_PAT_RIGHT cur =
            std::make_shared<ExprNode>(ASTNode::SyntaxType::Constant, scanner_.peekLexeme());
    scanner_.match(Token::Tag::Integer);
    return std::move(cur);
}
