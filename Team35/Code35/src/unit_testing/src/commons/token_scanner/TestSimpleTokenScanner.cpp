#include <vector>

#include "catch.hpp"
#include "../../TestHelper.h"
#include "../lexer/MockLexer.h"
#include "commons/lexer/ILexer.h"
#include "commons/token_scanner/SimpleTokenScanner.h"
#include "commons/token/Token.h"

TEST_CASE("1. test isName method") {
    Token eof(Token::Tag::EndOfFile);
    Token x("x", Token::Tag::Name);
    Token call(Token::Tag::Call);
    Token read(Token::Tag::Read);
    Token print(Token::Tag::Print);
    Token procedure(Token::Tag::Procedure);
    Token else_(Token::Tag::Else);
    Token then(Token::Tag::Then);
    Token if_(Token::Tag::If);
    Token while_(Token::Tag::While);
    Token constant("1", Token::Tag::Integer);
    Token semiColon(";", Token::Tag::SemiColon);

    SECTION("1.1. isName method returns true for valid name") {
        std::vector<Token> tokens = {eof, x, call, read, print, procedure, else_, then, if_, while_};
        std::unique_ptr<ILexer> lexer = std::make_unique<MockLexer>(tokens);
        SimpleTokenScanner scanner(std::move(lexer));
        requireTrue(scanner.isName());
        scanner.next();
        requireTrue(scanner.isName());
        scanner.next();
        requireTrue(scanner.isName());
        scanner.next();
        requireTrue(scanner.isName());
        scanner.next();
        requireTrue(scanner.isName());
        scanner.next();
        requireTrue(scanner.isName());
        scanner.next();
        requireTrue(scanner.isName());
        scanner.next();
        requireTrue(scanner.isName());
        scanner.next();
        requireTrue(scanner.isName());
        scanner.next();
        requireFalse(scanner.isName());
    }

    SECTION("1.2. isName method returns false for invalid name") {
        std::vector<Token> tokens = {eof, constant, semiColon};
        std::unique_ptr<ILexer> lexer = std::make_unique<MockLexer>(tokens);
        SimpleTokenScanner scanner(std::move(lexer));
        requireFalse(scanner.isName());
        scanner.next();
        requireFalse(scanner.isName());
        scanner.next();
        requireFalse(scanner.isName());
    }
}

TEST_CASE("2. test isAssign method") {
    Token eof(Token::Tag::EndOfFile);
    Token x("x", Token::Tag::Name);
    Token assign("=", Token::Tag::Assignment);
    Token constant("1", Token::Tag::Integer);
    Token semiColon(";", Token::Tag::SemiColon);

    SECTION("2.1. isAssign method returns true for valid assign") {
        std::vector<Token> tokens = {eof, semiColon, constant, assign, x};
        std::unique_ptr<ILexer> lexer = std::make_unique<MockLexer>(tokens);
        SimpleTokenScanner scanner(std::move(lexer));
        requireTrue(scanner.isAssign());
    }

    SECTION("2.2. isAssign method returns false for invalid assign") {
        std::vector<Token> tokens = {eof, x, constant, semiColon};
        std::unique_ptr<ILexer> lexer = std::make_unique<MockLexer>(tokens);
        SimpleTokenScanner scanner(std::move(lexer));
        requireFalse(scanner.isAssign());
    }
}

TEST_CASE("3. test isCondExprSeparatedByLogicalOperator method") {
    Token eof(Token::Tag::EndOfFile);
    Token x("x", Token::Tag::Name);
    Token lParen("(", Token::Tag::LParen);
    Token rParen(")", Token::Tag::RParen);
    Token logicalAnd("&&", Token::Tag::LogicalAnd);
    Token logicalOr("||", Token::Tag::LogicalOr);
    Token constant("1", Token::Tag::Integer);
    Token semiColon(";", Token::Tag::SemiColon);
    Token minus("-", Token::Tag::Minus);

    SECTION("3.1. isCondExprSeparatedByLogicalOperator method returns true for valid conditional expression") {
        std::vector<Token> tokens = {eof, rParen, constant, lParen, logicalAnd, rParen, constant, lParen};
        std::unique_ptr<ILexer> lexer = std::make_unique<MockLexer>(tokens);
        SimpleTokenScanner scanner(std::move(lexer));
        requireTrue(scanner.isCondExprSeparatedByLogicalOperator());
    }

    SECTION("3.2. isCondExprSeparatedByLogicalOperator method returns false for invalid conditional expression") {
        std::vector<Token> tokens = {eof, rParen, constant, lParen, minus, rParen, constant, lParen};
        std::unique_ptr<ILexer> lexer = std::make_unique<MockLexer>(tokens);
        SimpleTokenScanner scanner(std::move(lexer));
        requireFalse(scanner.isCondExprSeparatedByLogicalOperator());
    }

    SECTION(
        "3.3. isCondExprSeparatedByLogicalOperator method returns true for valid conditional expression, nested parentheses") {
        std::vector<Token>
            tokens = {eof, rParen, constant, lParen, logicalAnd, rParen, rParen, constant, lParen, lParen};
        std::unique_ptr<ILexer> lexer = std::make_unique<MockLexer>(tokens);
        SimpleTokenScanner scanner(std::move(lexer));
        requireTrue(scanner.isCondExprSeparatedByLogicalOperator());
    }
}
