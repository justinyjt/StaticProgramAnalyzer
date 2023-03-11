#include <memory>
#include <vector>

#include "catch.hpp"
#include "../../TestHelper.h"
#include "../lexer/MockLexer.h"
#include "commons/lexer/ILexer.h"
#include "commons/token_scanner/TokenScanner.h"
#include "commons/token/Token.h"

TEST_CASE("1. test token scanner match") {
    Token eof(Token::Tag::EndOfFile);
    Token x("x", Token::Tag::Name);
    Token y("y", Token::Tag::Name);
    Token semiColon(";", Token::Tag::SemiColon);
    Token assign("=", Token::Tag::Assign);
    Token constant("1", Token::Tag::Integer);

    SECTION("1.1. match valid token") {
        std::vector<Token> tokens = {eof, semiColon, x, y};
        std::unique_ptr<ILexer> lexer = std::make_unique<MockLexer>(tokens);
        TokenScanner scanner(std::move(lexer));
        requireTrue(scanner.match(Token::Tag::Name));
        requireTrue(scanner.match(Token::Tag::Name));
        requireTrue(scanner.match(Token::Tag::SemiColon));
        requireTrue(scanner.match(Token::Tag::EndOfFile));
        requireTrue(scanner.match(Token::Tag::EndOfFile));
    }

    SECTION("1.2. match invalid token") {
        std::vector<Token> tokens = {eof, semiColon, x, y};
        std::unique_ptr<ILexer> lexer = std::make_unique<MockLexer>(tokens);
        TokenScanner scanner(std::move(lexer));
        requireFalse(scanner.match(Token::Tag::Integer));
        requireFalse(scanner.match(Token::Tag::Assign));
        requireFalse(scanner.match(Token::Tag::EndOfFile));
        requireTrue(scanner.peek(Token::Tag::Name));
        Lexeme expectedLexeme = "y";
        requireEqual(scanner.peekLexeme(), expectedLexeme);
    }
}

TEST_CASE("2. test token scanner next") {
    Token eof(Token::Tag::EndOfFile);
    Token x("x", Token::Tag::Name);
    Token y("y", Token::Tag::Name);
    Token semiColon(";", Token::Tag::SemiColon);
    Token assign("=", Token::Tag::Assign);
    Token constant("1", Token::Tag::Integer);

    SECTION("2.1. non-eof next token") {
        std::vector<Token> tokens = {eof, semiColon, x, y};
        std::unique_ptr<ILexer> lexer = std::make_unique<MockLexer>(tokens);
        TokenScanner scanner(std::move(lexer));
        requireTrue(scanner.peek(Token::Tag::Name));
        requireTrue(scanner.next());
        requireTrue(scanner.peek(Token::Tag::Name));
        requireTrue(scanner.next());
        requireTrue(scanner.peek(Token::Tag::SemiColon));
        requireTrue(scanner.next());
        requireTrue(scanner.peek(Token::Tag::EndOfFile));
    }

    SECTION("2.2. last eof token") {
        std::vector<Token> tokens = {eof};
        std::unique_ptr<ILexer> lexer = std::make_unique<MockLexer>(tokens);
        TokenScanner scanner(std::move(lexer));
        requireTrue(scanner.peek(Token::Tag::EndOfFile));
        requireFalse(scanner.next());
    }
}

TEST_CASE("3. test token scanner offset") {
    Token eof(Token::Tag::EndOfFile);
    Token x("x", Token::Tag::Name);
    Token y("y", Token::Tag::Name);
    Token semiColon(";", Token::Tag::SemiColon);
    Token assign("=", Token::Tag::Assign);
    Token constant("1", Token::Tag::Integer);

    SECTION("3.1. valid offset") {
        std::vector<Token> tokens = {eof, semiColon, x, assign, y};
        std::unique_ptr<ILexer> lexer = std::make_unique<MockLexer>(tokens);
        TokenScanner scanner(std::move(lexer));
        requireTrue(scanner.peekOffset(Token::Tag::SemiColon, 3));
        requireTrue(scanner.isOffsetValid(3));
        requireTrue(scanner.peekOffset(Token::Tag::Assign, 1));
        requireTrue(scanner.isOffsetValid(1));
        requireTrue(scanner.peekOffset(Token::Tag::Name, 0));
        requireTrue(scanner.isOffsetValid(0));
    }

    SECTION("3.2. valid offset, invalid tag") {
        std::vector<Token> tokens = {eof, semiColon, x, assign, y};
        std::unique_ptr<ILexer> lexer = std::make_unique<MockLexer>(tokens);
        TokenScanner scanner(std::move(lexer));
        requireFalse(scanner.peekOffset(Token::Tag::Name, 3));
        requireTrue(scanner.isOffsetValid(3));
        requireFalse(scanner.peekOffset(Token::Tag::Plus, 1));
        requireTrue(scanner.isOffsetValid(1));
    }

    SECTION("3.3. invalid offset") {
        std::vector<Token> tokens = {eof, semiColon, x, assign, y};
        std::unique_ptr<ILexer> lexer = std::make_unique<MockLexer>(tokens);
        TokenScanner scanner(std::move(lexer));
        requireFalse(scanner.peekOffset(Token::Tag::SemiColon, 5));
        requireFalse(scanner.isOffsetValid(5));
        requireFalse(scanner.peekOffset(Token::Tag::SemiColon, 7));
        requireFalse(scanner.isOffsetValid(7));
        requireTrue(scanner.peekOffset(Token::Tag::EndOfFile, 4));
        requireTrue(scanner.isOffsetValid(4));
        requireFalse(scanner.peekOffset(Token::Tag::SemiColon, -1));
        requireFalse(scanner.isOffsetValid(-1));
    }
}

TEST_CASE("4. test token scanner save / restore state") {
    Token eof(Token::Tag::EndOfFile);
    Token x("x", Token::Tag::Name);
    Token y("y", Token::Tag::Name);
    Token semiColon(";", Token::Tag::SemiColon);
    Token assign("=", Token::Tag::Assign);
    Token constant("1", Token::Tag::Integer);

    SECTION("4.1. no saved state, go to start state") {
        std::vector<Token> tokens = {eof, semiColon, x, y};
        std::unique_ptr<ILexer> lexer = std::make_unique<MockLexer>(tokens);
        TokenScanner scanner(std::move(lexer));
        scanner.next();
        scanner.next();
        requireTrue(scanner.peek(Token::Tag::SemiColon));
        scanner.restoreState();
        requireTrue(scanner.peek(Token::Tag::Name));
        Lexeme expectedLexeme = "y";
        requireEqual(scanner.peekLexeme(), expectedLexeme);
    }

    SECTION("4.2. saved state, go to saved state") {
        std::vector<Token> tokens = {eof, assign, semiColon, x, y};
        std::unique_ptr<ILexer> lexer = std::make_unique<MockLexer>(tokens);
        TokenScanner scanner(std::move(lexer));
        scanner.next();
        scanner.next();
        requireTrue(scanner.peek(Token::Tag::SemiColon));
        scanner.saveState();
        requireTrue(scanner.peek(Token::Tag::SemiColon));
        scanner.next();
        requireTrue(scanner.peek(Token::Tag::Assign));
        scanner.restoreState();
        requireTrue(scanner.peek(Token::Tag::SemiColon));
    }
}
