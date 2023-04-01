#include "ExprParser.h"

#include <memory>
#include <utility>

#include "ExprNode.h"

using std::unique_ptr;

ExprParser::ExprParser(TokenScanner &scanner) : scanner_(scanner) {}

ASSIGN_PAT_RIGHT ExprParser::parseExpr() {
    ASSIGN_PAT_RIGHT firstOp = parseTerm();
    while (scanner_.peek(Token::Tag::Plus) || scanner_.peek(Token::Tag::Minus)) {
        ASTNode::SyntaxType opType;
        std::string opLabel;
        if (scanner_.peek(Token::Tag::Plus)) {
            opType = ASTNode::SyntaxType::Plus;
            opLabel = ExprNode::plusAscii;
        } else {
            opType = ASTNode::SyntaxType::Minus;
            opLabel = ExprNode::minusAscii;
        }
        scanner_.next();
        ASSIGN_PAT_RIGHT op = std::make_shared<ExprNode>(opType, opLabel);
        ASSIGN_PAT_RIGHT secondOp = parseTerm();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        firstOp = std::move(op);
    }
    return std::move(firstOp);
}

ASSIGN_PAT_RIGHT ExprParser::parseTerm() {
    ASSIGN_PAT_RIGHT firstOp = parseFactor();
    while (scanner_.peek(Token::Tag::Multiply)
            || scanner_.peek(Token::Tag::Divide)
            || scanner_.peek(Token::Tag::Modulo)) {
        ASTNode::SyntaxType opType;
        std::string opLabel;
        if (scanner_.peek(Token::Tag::Multiply)) {
            opType = ASTNode::SyntaxType::Multiply;
            opLabel = ExprNode::multiplyAscii;
        } else if (scanner_.peek(Token::Tag::Divide)) {
            opType = ASTNode::SyntaxType::Divide;
            opLabel = ExprNode::divideAscii;
        } else {
            opType = ASTNode::SyntaxType::Modulo;
            opLabel = ExprNode::moduloAscii;
        }
        scanner_.next();
        ASSIGN_PAT_RIGHT op = std::make_shared<ExprNode>(opType, opLabel);
        ASSIGN_PAT_RIGHT secondOp = parseFactor();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        firstOp = std::move(op);
    }
    return std::move(firstOp);
}

ASSIGN_PAT_RIGHT ExprParser::parseFactor() {
    ASSIGN_PAT_RIGHT cur;
    if (scanner_.isName()) {
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
    ASSIGN_PAT_RIGHT cur =
            std::make_shared<ExprNode>(ASTNode::SyntaxType::Variable, scanner_.peekLexeme());
    scanner_.next();
    return std::move(cur);
}

ASSIGN_PAT_RIGHT ExprParser::parseInteger() {
    ASSIGN_PAT_RIGHT cur =
            std::make_shared<ExprNode>(ASTNode::SyntaxType::Constant, scanner_.peekLexeme());
    scanner_.match(Token::Tag::Integer);
    return std::move(cur);
}
