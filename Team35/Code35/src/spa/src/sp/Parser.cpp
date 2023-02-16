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
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(
        ASTNode::SyntaxType::Program,
        std::nullopt);
    if (scanner_.peek(Token::Tag::Name)) {
        return std::move(parseAssign());
    } else if (scanner_.peek(Token::Tag::Read)) {
        return std::move(parseRead());
    } else if (scanner_.peek(Token::Tag::Print)) {
        return std::move(parsePrint());
    } else {
        assert(false);
    }
    return std::move(cur);
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





