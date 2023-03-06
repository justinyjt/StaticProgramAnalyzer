#include <memory>
#include <utility>

#include "ExprParser.h"

using std::unique_ptr;

ExprParser::ExprParser(TokenScanner *scanner) { scanner_ = scanner; }

std::unique_ptr<ExprNode> ExprParser::parseExpr() {
    unique_ptr<ExprNode> firstOp = parseTerm();
    if (scanner_->match(Token::Tag::Plus)) {
        unique_ptr<ExprNode> op = std::make_unique<ExprNode>(ASTNode::SyntaxType::Plus, "+");
        unique_ptr<ExprNode> secondOp = parseExpr();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return std::move(op);
    } else if (scanner_->match(Token::Tag::Minus)) {
        unique_ptr<ExprNode> op = std::make_unique<ExprNode>(ASTNode::SyntaxType::Minus, "-");
        unique_ptr<ExprNode> secondOp = parseExpr();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return std::move(op);
    } else {
        return std::move(firstOp);
    }
}

std::unique_ptr<ExprNode> ExprParser::parseTerm() {
    unique_ptr<ExprNode> firstOp = parseFactor();
    if (scanner_->match(Token::Tag::Multiply)) {
        unique_ptr<ExprNode> op = std::make_unique<ExprNode>(ASTNode::SyntaxType::Multiply, "*");
        unique_ptr<ExprNode> secondOp = parseTerm();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return std::move(op);
    } else if (scanner_->match(Token::Tag::Divide)) {
        unique_ptr<ExprNode> op = std::make_unique<ExprNode>(ASTNode::SyntaxType::Divide, "/");
        unique_ptr<ExprNode> secondOp = parseTerm();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return std::move(op);
    } else if (scanner_->match(Token::Tag::Modulo)) {
        unique_ptr<ExprNode> op = std::make_unique<ExprNode>(ASTNode::SyntaxType::Modulo, "%");
        unique_ptr<ExprNode> secondOp = parseTerm();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return std::move(op);
    } else {
        return std::move(firstOp);
    }
}

std::unique_ptr<ExprNode> ExprParser::parseFactor() {
    unique_ptr<ExprNode> cur;
    if (scanner_->peek(Token::Tag::Name)) {
        cur = parseName();
    } else if (scanner_->peek(Token::Tag::Integer)) {
        cur = parseInteger();
    } else {
        scanner_->match(Token::Tag::LParen);
        cur = parseExpr();
        scanner_->match(Token::Tag::RParen);
    }
    return std::move(cur);
}

std::unique_ptr<ExprNode> ExprParser::parseName() {
    assert(scanner_->peek(Token::Tag::Name));
    unique_ptr<ExprNode> cur =
            std::make_unique<ExprNode>(ASTNode::SyntaxType::Variable, scanner_->peekLexeme());
    scanner_->match(Token::Tag::Name);
    return std::move(cur);
}

std::unique_ptr<ExprNode> ExprParser::parseInteger() {
    assert(scanner_->peek(Token::Tag::Integer));
    unique_ptr<ExprNode> cur =
            std::make_unique<ExprNode>(ASTNode::SyntaxType::Constant, scanner_->peekLexeme());
    scanner_->match(Token::Tag::Integer);
    return std::move(cur);
}

