#include <memory>
#include <utility>

#include "MockParser.h"
#include "commons/ASTNode.h"
using std::unique_ptr;

MockParser::MockParser(std::string src, const DesignExtractor &de, const MockLexer& lex) :
        lex_(std::move(std::make_unique<MockLexer>(lex))),
        src_(std::move(src)), de_((DesignExtractor &) de),
        cur_(Token::Tag::EndOfFile), isRead_(false) {}

int MockParser::peek(Token::Tag tag) {
    if (!isRead_) {
        cur_ = lex_->scan();
        isRead_ = true;
    }
    if (cur_.getTag() == Token::Tag::EndOfFile) {
        std::runtime_error("Error: Token List running out!");
    }
    return (cur_.getTag() == tag) ? 1 : 0;
}

int MockParser::accept(Token::Tag tag) {
    if (!isRead_) {
        cur_ = lex_->scan();
        isRead_ = true;
    }
    if (cur_.getTag() == tag) {
        if (cur_.getTag() != Token::Tag::EndOfFile) {
            cur_ = lex_->scan();
        }
        return 1;
    }
    return 0;
}

int MockParser::expect(Token::Tag tag) {
    if (accept(tag) == 1) {
        return 1;
    }
    std::runtime_error give_me_a_name("Error: unexpected token");
    return 0;
}

unique_ptr<ASTNode> MockParser::Parse() {
    unique_ptr<ASTNode> root = std::make_unique<ASTNode>
            (ASTNode::SyntaxType::program,
             std::nullopt);

    while (accept(Token::Tag::Procedure)) {
        root->addChild(parseProc());
    }
    expect(Token::Tag::EndOfFile);
    //  de_.extractProgram(std::move(root));
    return root;
}

unique_ptr<ASTNode> MockParser::parseProc() {
    peek(Token::Tag::Name);
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode>(
            ASTNode::SyntaxType::procedure,
            cur_.getLexeme());
    accept(Token::Tag::Name);
    cur->addChild(parseStmtLst());
    return cur;
}

unique_ptr<ASTNode> MockParser::parseStmtLst() {
    expect(Token::Tag::LBrace);
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode> (
            ASTNode::SyntaxType::stmtLst,
            std::nullopt);
    while (accept(Token::Tag::RBrace) != 1) {
        cur->addChild(parseStmt());
    }
    return cur;
}

unique_ptr<ASTNode> MockParser::parseStmt() {
    unique_ptr<ASTNode> cur;
    if (peek(Token::Tag::Name)) {
        cur = parseAssign();
    } else if (peek(Token::Tag::Read)) {
        cur = parseRead();
    } else if (peek(Token::Tag::Print)) {
        cur = parsePrint();
    } else {
        std::runtime_error("Unidentified Token");
    }
    return cur;
}

unique_ptr<ASTNode> MockParser::parseAssign() {
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode> (
            ASTNode::SyntaxType::assign,
            std::nullopt);

    cur->addChild(parseName());
    expect(Token::Tag::Assignment);
    cur->addChild(parseExpr());
    expect(Token::Tag::SemiColon);
    return cur;
}

unique_ptr<ASTNode> MockParser::parseRead() {
    accept(Token::Tag::Read);
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode> (
            ASTNode::SyntaxType::read,
            std::nullopt);

    cur->addChild(parseName());
    expect(Token::Tag::SemiColon);
    return cur;
}

unique_ptr<ASTNode> MockParser::parsePrint() {
    accept(Token::Tag::Print);
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode> (
            ASTNode::SyntaxType::print,
            std::nullopt);

    cur->addChild(parseName());
    expect(Token::Tag::SemiColon);
    return cur;
}

unique_ptr<ASTNode> MockParser::parseExpr() {
    unique_ptr<ASTNode> firstOp = parseTerm();
    if (accept(Token::Tag::Plus)) {
        unique_ptr<ASTNode> op = std::make_unique<ASTNode> (
                ASTNode::SyntaxType::plus,
                "+");
        unique_ptr<ASTNode> secondOp = parseExpr();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return op;
    } else if (accept(Token::Tag::Minus)) {
        unique_ptr<ASTNode> op = std::make_unique<ASTNode> (
                ASTNode::SyntaxType::minus,
                "-");
        unique_ptr<ASTNode> secondOp = parseExpr();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return op;
    } else {
        return firstOp;
    }
}

unique_ptr<ASTNode> MockParser::parseTerm() {
    unique_ptr<ASTNode> firstOp = parseFactor();
    if (accept(Token::Tag::Multiply)) {
        unique_ptr<ASTNode> op = std::make_unique<ASTNode> (
                ASTNode::SyntaxType::times,
                "*");
        unique_ptr<ASTNode> secondOp = parseTerm();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return op;
    } else if (accept(Token::Tag::Divide)) {
        unique_ptr<ASTNode> op = std::make_unique<ASTNode> (
                ASTNode::SyntaxType::div,
                "/");
        unique_ptr<ASTNode> secondOp = parseTerm();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return op;
    } else if (accept(Token::Tag::Modulo)) {
        unique_ptr<ASTNode> op = std::make_unique<ASTNode> (
                ASTNode::SyntaxType::mod,
                "%");
        unique_ptr<ASTNode> secondOp = parseTerm();
        op->addChild(std::move(firstOp));
        op->addChild(std::move(secondOp));

        return op;
    } else {
        return firstOp;
    }
}

unique_ptr<ASTNode> MockParser::parseFactor() {
    unique_ptr<ASTNode> cur;
    if (peek(Token::Tag::Name)) {
        cur = parseName();
    } else if (peek(Token::Tag::Integer)) {
        cur = parseInteger();
    } else if (accept(Token::Tag::LParen)) {
        cur = parseExpr();
        expect(Token::Tag::RParen);
    } else {
        std::runtime_error("Factor: syntax error");
    }
    return cur;
}

unique_ptr<ASTNode> MockParser::parseName() {
    peek(Token::Tag::Name);
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode> (
            ASTNode::SyntaxType::var,
            cur_.getLexeme());
    accept(Token::Tag::Name);
    return cur;
}

unique_ptr<ASTNode> MockParser::parseInteger() {
    peek(Token::Tag::Integer);
    unique_ptr<ASTNode> cur = std::make_unique<ASTNode> (
            ASTNode::SyntaxType::constVal,
            cur_.getLexeme());
    accept(Token::Tag::Integer);
    return cur;
}




