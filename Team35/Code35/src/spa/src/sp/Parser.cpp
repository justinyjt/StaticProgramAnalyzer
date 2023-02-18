#include "Parser.h"

#include <memory>
#include <utility>
#include <cassert>

#include "commons/ASTNode.h"

using std::unique_ptr;

Parser::Parser(TokenLst token_lst) : scanner_(std::move(token_lst)) {}

unique_ptr<ASTNode> Parser::Parse() {
    unique_ptr<ASTNode> root = std::make_unique<ASTNode>(ASTNode::SyntaxType::Program, std::nullopt);

    while (scanner_.match(Token::Tag::Procedure)) {
        root->addChild(parseProc());
    }

    scanner_.match(Token::Tag::EndOfFile);
    return std::move(root);
}

unique_ptr<ASTNode> Parser::parseProc() {
    assert(scanner_.peek(Token::Tag::Name));
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(ASTNode::SyntaxType::Procedure, scanner_.peekLexeme());

    scanner_.match(Token::Tag::Name);
    cur->addChild(parseStmtLst());
    return std::move(cur);
}

unique_ptr<ASTNode> Parser::parseStmtLst() {
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(ASTNode::SyntaxType::StmtLst, std::nullopt);

    // Assuming no recursion procedure
    scanner_.match(Token::Tag::LBrace);
    while (scanner_.match(Token::Tag::RBrace) != 1) {
        cur->addChild(parseStmt());
    }
    return std::move(cur);
}

unique_ptr<ASTNode> Parser::parseStmt() {
    if (scanner_.isAssign()) {
        return std::move(parseAssign());
    } else if (scanner_.peek(Token::Tag::Read)) {
        return std::move(parseRead());
    } else if (scanner_.peek(Token::Tag::Print)) {
        return std::move(parsePrint());
    } else if (scanner_.peek(Token::Tag::If)) {
        return std::move(parseIf());
    } else {
        assert(scanner_.peek(Token::Tag::While));
        return std::move(parseWhile());
    }
}

unique_ptr<ASTNode> Parser::parseAssign() {
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(ASTNode::SyntaxType::Assign, std::nullopt);

    cur->addChild(parseName());

    scanner_.match(Token::Tag::Assignment);

    cur->addChild(parseExpr());

    scanner_.match(Token::Tag::SemiColon);
    return std::move(cur);
}

unique_ptr<ASTNode> Parser::parseRead() {
    scanner_.match(Token::Tag::Read);
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(ASTNode::SyntaxType::Read, std::nullopt);

    cur->addChild(parseName());

    scanner_.match(Token::Tag::SemiColon);
    return std::move(cur);
}

unique_ptr<ASTNode> Parser::parsePrint() {
    scanner_.match(Token::Tag::Print);
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(ASTNode::SyntaxType::Print, std::nullopt);

    cur->addChild(parseName());

    scanner_.match(Token::Tag::SemiColon);
    return std::move(cur);
}

unique_ptr<ASTNode> Parser::parseIf() {
    scanner_.match(Token::Tag::If);
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(ASTNode::SyntaxType::If, std::nullopt);

    scanner_.match(Token::Tag::LParen);
    cur->addChild(parseCondExpr());
    scanner_.match(Token::Tag::RParen);

    scanner_.match(Token::Tag::Then);
    cur->addChild(parseStmtLst());

    scanner_.match(Token::Tag::Else);
    cur->addChild(parseStmtLst());

    return std::move(cur);
}

unique_ptr<ASTNode> Parser::parseWhile() {
    scanner_.match(Token::Tag::While);
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(ASTNode::SyntaxType::While, std::nullopt);

    scanner_.match(Token::Tag::LParen);
    cur->addChild(parseCondExpr());
    scanner_.match(Token::Tag::RParen);

    cur->addChild(parseStmtLst());

    return std::move(cur);
}

unique_ptr<ASTNode> Parser::parseCondExpr() {
    if (scanner_.peek(Token::Tag::LParen) && scanner_.isCondExprSeparatedByLogicalOperator()) {
        scanner_.match(Token::Tag::LParen);
        unique_ptr<ASTNode> expr1 = parseCondExpr();
        scanner_.match(Token::Tag::RParen);

        if (scanner_.peek(Token::Tag::LogicalAnd)) {
            scanner_.next();
            unique_ptr<ASTNode> op = std::make_unique<ASTNode>(ASTNode::SyntaxType::LogicalAnd, "&&");

            scanner_.match(Token::Tag::LParen);
            unique_ptr<ASTNode> expr2 = parseCondExpr();
            scanner_.match(Token::Tag::RParen);

            op->addChild(std::move(expr1));
            op->addChild(std::move(expr2));
            return std::move(op);
        } else {  // LogicalOr, checked by SyntaxValidator
            assert(scanner_.peek(Token::Tag::LogicalOr));
            scanner_.next();
            unique_ptr<ASTNode> op = std::make_unique<ASTNode>(ASTNode::SyntaxType::LogicalOr, "||");

            scanner_.match(Token::Tag::LParen);
            unique_ptr<ASTNode> expr2 = parseCondExpr();
            scanner_.match(Token::Tag::RParen);

            op->addChild(std::move(expr1));
            op->addChild(std::move(expr2));

            return std::move(op);
        }
    } else {
        if (scanner_.match(Token::Tag::LogicalNot)) {
            unique_ptr<ASTNode> op = std::make_unique<ASTNode>(ASTNode::SyntaxType::LogicalNot, "!");

            scanner_.match(Token::Tag::LParen);
            unique_ptr<ASTNode> expr1 = parseCondExpr();
            scanner_.match(Token::Tag::RParen);

            op->addChild(std::move(expr1));

            return std::move(op);
        } else {
            return std::move(parseRelExpr());
        }
    }
}

unique_ptr<ASTNode> Parser::parseRelExpr() {
    unique_ptr<ASTNode> factor1 = parseRelFactor();
    if (scanner_.peek(Token::Tag::GreaterThan)) {
        scanner_.next();
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(ASTNode::SyntaxType::GreaterThan, ">");
        unique_ptr<ASTNode> factor2 = parseRelFactor();

        op->addChild(std::move(factor1));
        op->addChild(std::move(factor2));

        return std::move(op);
    } else if (scanner_.peek(Token::Tag::GreaterThanEqualTo)) {
        scanner_.next();
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(ASTNode::SyntaxType::GreaterThanEqualTo, ">=");
        unique_ptr<ASTNode> factor2 = parseRelFactor();

        op->addChild(std::move(factor1));
        op->addChild(std::move(factor2));

        return std::move(op);
    } else if (scanner_.peek(Token::Tag::LessThan)) {
        scanner_.next();
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(ASTNode::SyntaxType::LessThan, "<");
        unique_ptr<ASTNode> factor2 = parseRelFactor();

        op->addChild(std::move(factor1));
        op->addChild(std::move(factor2));

        return std::move(op);
    } else if (scanner_.peek(Token::Tag::LessThanEqualTo)) {
        scanner_.next();
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(ASTNode::SyntaxType::LessThanEqualTo, "<=");
        unique_ptr<ASTNode> factor2 = parseRelFactor();

        op->addChild(std::move(factor1));
        op->addChild(std::move(factor2));

        return std::move(op);
    } else if (scanner_.peek(Token::Tag::Equivalence)) {
        scanner_.next();
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(ASTNode::SyntaxType::Equivalence, "==");
        unique_ptr<ASTNode> factor2 = parseRelFactor();

        op->addChild(std::move(factor1));
        op->addChild(std::move(factor2));

        return std::move(op);
    } else {
        assert(scanner_.peek(Token::Tag::NotEqual));
        scanner_.next();
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(ASTNode::SyntaxType::NotEqual, "!=");
        unique_ptr<ASTNode> factor2 = parseRelFactor();

        op->addChild(std::move(factor1));
        op->addChild(std::move(factor2));

        return std::move(op);
    }
}

unique_ptr<ASTNode> Parser::parseRelFactor() {
    return std::move(parseExpr());
}

unique_ptr<ASTNode> Parser::parseExpr() {
    unique_ptr<ASTNode> firstOp = parseTerm();
    if (scanner_.match(Token::Tag::Plus)) {
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(ASTNode::SyntaxType::Plus, "+");
        unique_ptr<ASTNode> secondOp = parseExpr();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return std::move(op);
    } else if (scanner_.match(Token::Tag::Minus)) {
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(ASTNode::SyntaxType::Minus, "-");
        unique_ptr<ASTNode> secondOp = parseExpr();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return std::move(op);
    } else {
        return std::move(firstOp);
    }
}

unique_ptr<ASTNode> Parser::parseTerm() {
    unique_ptr<ASTNode> firstOp = parseFactor();
    if (scanner_.match(Token::Tag::Multiply)) {
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(ASTNode::SyntaxType::Multiply, "*");
        unique_ptr<ASTNode> secondOp = parseTerm();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return std::move(op);
    } else if (scanner_.match(Token::Tag::Divide)) {
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(ASTNode::SyntaxType::Divide, "/");
        unique_ptr<ASTNode> secondOp = parseTerm();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return std::move(op);
    } else if (scanner_.match(Token::Tag::Modulo)) {
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(ASTNode::SyntaxType::Modulo, "%");
        unique_ptr<ASTNode> secondOp = parseTerm();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return std::move(op);
    } else {
        return std::move(firstOp);
    }
}

unique_ptr<ASTNode> Parser::parseFactor() {
    unique_ptr<ASTNode> cur;
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

unique_ptr<ASTNode> Parser::parseName() {
    assert(scanner_.peek(Token::Tag::Name));
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(ASTNode::SyntaxType::Variable, scanner_.peekLexeme());
    scanner_.match(Token::Tag::Name);
    return std::move(cur);
}

unique_ptr<ASTNode> Parser::parseInteger() {
    assert(scanner_.peek(Token::Tag::Integer));
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(ASTNode::SyntaxType::Constant, scanner_.peekLexeme());
    scanner_.match(Token::Tag::Integer);
    return std::move(cur);
}





