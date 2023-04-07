#include "Parser.h"

#include <memory>
#include <utility>

#include "commons/ASTNode.h"
#include "commons/expr_parser/ExprParser.h"

using std::shared_ptr;

Parser::Parser(SimpleTokenScanner &scanner) : scanner_(scanner) {}

shared_ptr<ASTNode> Parser::parse() {
    shared_ptr<ASTNode> root = std::make_shared<ASTNode>(ASTNode::SyntaxType::Program, std::nullopt);

    while (scanner_.match(Token::Tag::Procedure)) {
        root->addChild(parseProc());
    }

    scanner_.match(Token::Tag::EndOfFile);
    return std::move(root);
}

shared_ptr<ASTNode> Parser::parseProc() {
    shared_ptr<ASTNode> cur = std::make_shared<ASTNode>(ASTNode::SyntaxType::Procedure, scanner_.peekLexeme());
    scanner_.next();
    cur->addChild(parseStmtLst());
    return std::move(cur);
}

shared_ptr<ASTNode> Parser::parseStmtLst() {
    shared_ptr<ASTNode> cur = std::make_shared<ASTNode>(ASTNode::SyntaxType::StmtLst, std::nullopt);

    scanner_.match(Token::Tag::LBrace);
    while (scanner_.match(Token::Tag::RBrace) == 0) {
        cur->addChild(parseStmt());
    }
    return std::move(cur);
}

shared_ptr<ASTNode> Parser::parseStmt() {
    if (scanner_.isAssign()) {
        return std::move(parseAssign());
    } else if (scanner_.peek(Token::Tag::Read)) {
        return std::move(parseRead());
    } else if (scanner_.peek(Token::Tag::Print)) {
        return std::move(parsePrint());
    } else if (scanner_.peek(Token::Tag::If)) {
        return std::move(parseIf());
    } else if (scanner_.peek(Token::Tag::While)) {
        return std::move(parseWhile());
    } else {
        return std::move(parseCall());
    }
}

shared_ptr<ASTNode> Parser::parseAssign() {
    shared_ptr<ASTNode> cur = std::make_shared<ASTNode>(ASTNode::SyntaxType::Assign, std::nullopt);

    cur->addChild(parseName());

    scanner_.match(Token::Tag::Assignment);

    cur->addChild(parseExpr());

    scanner_.match(Token::Tag::SemiColon);
    return std::move(cur);
}

shared_ptr<ASTNode> Parser::parseRead() {
    scanner_.match(Token::Tag::Read);
    shared_ptr<ASTNode> cur = std::make_shared<ASTNode>(ASTNode::SyntaxType::Read, std::nullopt);

    cur->addChild(parseName());

    scanner_.match(Token::Tag::SemiColon);
    return std::move(cur);
}

shared_ptr<ASTNode> Parser::parsePrint() {
    scanner_.match(Token::Tag::Print);
    shared_ptr<ASTNode> cur = std::make_shared<ASTNode>(ASTNode::SyntaxType::Print, std::nullopt);

    cur->addChild(parseName());

    scanner_.match(Token::Tag::SemiColon);
    return std::move(cur);
}

shared_ptr<ASTNode> Parser::parseIf() {
    scanner_.match(Token::Tag::If);
    shared_ptr<ASTNode> cur = std::make_shared<ASTNode>(ASTNode::SyntaxType::If, std::nullopt);

    scanner_.match(Token::Tag::LParen);
    cur->addChild(parseCondExpr());
    scanner_.match(Token::Tag::RParen);

    scanner_.match(Token::Tag::Then);
    cur->addChild(parseStmtLst());

    scanner_.match(Token::Tag::Else);
    cur->addChild(parseStmtLst());

    return std::move(cur);
}

shared_ptr<ASTNode> Parser::parseWhile() {
    scanner_.match(Token::Tag::While);
    shared_ptr<ASTNode> cur = std::make_shared<ASTNode>(ASTNode::SyntaxType::While, std::nullopt);

    scanner_.match(Token::Tag::LParen);
    cur->addChild(parseCondExpr());
    scanner_.match(Token::Tag::RParen);

    cur->addChild(parseStmtLst());

    return std::move(cur);
}

shared_ptr<ASTNode> Parser::parseCall() {
    scanner_.match(Token::Tag::Call);
    shared_ptr<ASTNode> cur = std::make_unique<ASTNode>(ASTNode::SyntaxType::Call, std::nullopt);
    cur->addChild(parseName());
    scanner_.match(Token::Tag::SemiColon);
    return std::move(cur);
}

shared_ptr<ASTNode> Parser::parseCondExpr() {
    if (scanner_.peek(Token::Tag::LParen) && scanner_.isCondExprSeparatedByLogicalOperator()) {
        scanner_.match(Token::Tag::LParen);
        shared_ptr<ASTNode> expr1 = parseCondExpr();
        scanner_.match(Token::Tag::RParen);
        shared_ptr<ASTNode> op;
        if (scanner_.match(Token::Tag::LogicalAnd)) {
            op = std::make_shared<ASTNode>(ASTNode::SyntaxType::LogicalAnd, "&&");
        } else {  // LogicalOr, checked by SourceValidator
            scanner_.next();
            op = std::make_shared<ASTNode>(ASTNode::SyntaxType::LogicalOr, "||");
        }

        scanner_.match(Token::Tag::LParen);
        shared_ptr<ASTNode> expr2 = parseCondExpr();
        scanner_.match(Token::Tag::RParen);

        op->addChild(std::move(expr1));
        op->addChild(std::move(expr2));
        return std::move(op);
    } else {
        if (scanner_.match(Token::Tag::LogicalNot)) {
            shared_ptr<ASTNode> op = std::make_shared<ASTNode>(ASTNode::SyntaxType::LogicalNot, "!");

            scanner_.match(Token::Tag::LParen);
            shared_ptr<ASTNode> expr = parseCondExpr();
            scanner_.match(Token::Tag::RParen);

            op->addChild(std::move(expr));
            return std::move(op);
        } else {
            return std::move(parseRelExpr());
        }
    }
}

shared_ptr<ASTNode> Parser::parseRelExpr() {
    shared_ptr<ASTNode> factor1 = parseRelFactor();
    shared_ptr<ASTNode> op;
    if (scanner_.match(Token::Tag::GreaterThan)) {
        op = std::make_shared<ASTNode>(ASTNode::SyntaxType::GreaterThan, ">");
    } else if (scanner_.match(Token::Tag::GreaterThanEqualTo)) {
        op = std::make_shared<ASTNode>(ASTNode::SyntaxType::GreaterThanEqualTo, ">=");
    } else if (scanner_.match(Token::Tag::LessThan)) {
        op = std::make_shared<ASTNode>(ASTNode::SyntaxType::LessThan, "<");
    } else if (scanner_.match(Token::Tag::LessThanEqualTo)) {
        op = std::make_shared<ASTNode>(ASTNode::SyntaxType::LessThanEqualTo, "<=");
    } else if (scanner_.match(Token::Tag::Equivalence)) {
        op = std::make_shared<ASTNode>(ASTNode::SyntaxType::Equivalence, "==");
    } else {  // NotEqual, checked by SourceValidator
        scanner_.next();
        op = std::make_shared<ASTNode>(ASTNode::SyntaxType::NotEqual, "!=");
    }
    shared_ptr<ASTNode> factor2 = parseRelFactor();

    op->addChild(std::move(factor1));
    op->addChild(std::move(factor2));

    return std::move(op);
}

shared_ptr<ASTNode> Parser::parseRelFactor() {
    return std::move(parseExpr());
}

shared_ptr<ASTNode> Parser::parseExpr() {
    std::shared_ptr<ExprParser> exprParser = std::make_shared<ExprParser>(scanner_);
    return std::move(exprParser->parseExpr());
}

shared_ptr<ASTNode> Parser::parseName() {
    shared_ptr<ASTNode> cur = std::make_shared<ASTNode>(ASTNode::SyntaxType::Variable, scanner_.peekLexeme());
    scanner_.next();
    return std::move(cur);
}
