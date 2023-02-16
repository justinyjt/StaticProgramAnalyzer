#include <memory>
#include <utility>
#include <cassert>
#include <deque>

#include "Parser.h"
#include "commons/ASTNode.h"
using std::unique_ptr;

Parser::Parser(IParser::PROGRAM src, std::unique_ptr<PKBWriter> pkb, std::deque<std::unique_ptr<Token>> tokenLst,
               bool flagExtract) :
    tokenLst_(std::move(tokenLst)), flagExtract_(flagExtract),
    src_(std::move(src)), de_(std::make_unique<DesignExtractor>(DesignExtractor(std::move(pkb)))) {}

int Parser::peek(Token::Tag tag) {
    return tokenLst_.back()->getTag() == tag ? 1 : 0;
}

Lexeme Parser::peekLexeme() {
    return tokenLst_.back()->getLexeme();
}

int Parser::accept(Token::Tag tag) {
    if (tokenLst_.back()->getTag() == tag) {
        tokenLst_.pop_back();
        return 1;
    }
    return 0;
}

unique_ptr<ASTNode> Parser::Parse() {
    unique_ptr<ASTNode> root = std::make_unique<ASTNode>
        (ASTNode::SyntaxType::Program,
         std::nullopt);

    while (accept(Token::Tag::Procedure)) {
        root->addChild(parseProc());
    }

    accept(Token::Tag::EndOfFile);
    return flagExtract_ ? de_->extractProgram(std::move(root)) : std::move(root);
}

unique_ptr<ASTNode> Parser::parseProc() {
    assert(peek(Token::Tag::Name) == 1);
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(
        ASTNode::SyntaxType::Procedure,
        peekLexeme());

    accept(Token::Tag::Name);
    cur->addChild(parseStmtLst());
    return std::move(cur);
}

unique_ptr<ASTNode> Parser::parseStmtLst() {
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(
        ASTNode::SyntaxType::StmtLst,
        std::nullopt);
    // Assuming no recursion procedure

    accept(Token::Tag::LBrace);
    while (accept(Token::Tag::RBrace) != 1) {
        cur->addChild(parseStmt());
    }
    return std::move(cur);
}

unique_ptr<ASTNode> Parser::parseStmt() {
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(
        ASTNode::SyntaxType::Program,
        std::nullopt);
    if (peek(Token::Tag::Name)) {
        return std::move(parseAssign());
    } else if (peek(Token::Tag::Read)) {
        return std::move(parseRead());
    } else if (peek(Token::Tag::Print)) {
        return std::move(parsePrint());
    } else {
        assert(false);
    }
    return std::move(cur);
}

unique_ptr<ASTNode> Parser::parseAssign() {
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(
        ASTNode::SyntaxType::Assign,
        std::nullopt);

    cur->addChild(parseName());

    accept(Token::Tag::Assignment);

    cur->addChild(parseExpr());

    accept(Token::Tag::SemiColon);
    return std::move(cur);
}

unique_ptr<ASTNode> Parser::parseRead() {
    accept(Token::Tag::Read);
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(
        ASTNode::SyntaxType::Read,
        std::nullopt);

    cur->addChild(parseName());

    accept(Token::Tag::SemiColon);
    return std::move(cur);
}

unique_ptr<ASTNode> Parser::parsePrint() {
    accept(Token::Tag::Print);
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(
        ASTNode::SyntaxType::Print,
        std::nullopt);

    cur->addChild(parseName());

    accept(Token::Tag::SemiColon);
    return std::move(cur);
}

unique_ptr<ASTNode> Parser::parseExpr() {
    unique_ptr<ASTNode> firstOp = parseTerm();
    if (accept(Token::Tag::Plus)) {
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(
            ASTNode::SyntaxType::Plus,
            "+");
        unique_ptr<ASTNode> secondOp = parseExpr();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return std::move(op);
    } else if (accept(Token::Tag::Minus)) {
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(
            ASTNode::SyntaxType::Minus,
            "-");
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
    if (accept(Token::Tag::Multiply)) {
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(
            ASTNode::SyntaxType::Multiply,
            "*");
        unique_ptr<ASTNode> secondOp = parseTerm();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return std::move(op);
    } else if (accept(Token::Tag::Divide)) {
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(
            ASTNode::SyntaxType::Divide,
            "/");
        unique_ptr<ASTNode> secondOp = parseTerm();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return std::move(op);
    } else if (accept(Token::Tag::Modulo)) {
        unique_ptr<ASTNode> op = std::make_unique<ASTNode>(
            ASTNode::SyntaxType::Modulo,
            "%");
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
    if (peek(Token::Tag::Name)) {
        cur = parseName();
    } else if (peek(Token::Tag::Integer)) {
        cur = parseInteger();
    } else {
        accept(Token::Tag::LParen);
        cur = parseExpr();
        accept(Token::Tag::RParen);
    }
    return std::move(cur);
}

unique_ptr<ASTNode> Parser::parseName() {
    assert(peek(Token::Tag::Name) == 1);
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(
        ASTNode::SyntaxType::Variable,
        peekLexeme());
    accept(Token::Tag::Name);
    return std::move(cur);
}

unique_ptr<ASTNode> Parser::parseInteger() {
    assert(peek(Token::Tag::Integer) == 1);
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(
        ASTNode::SyntaxType::Constant,
        peekLexeme());
    accept(Token::Tag::Integer);
    return std::move(cur);
}





