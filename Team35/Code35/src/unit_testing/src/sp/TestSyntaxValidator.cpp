/*
 * 1. Add more negative cases
 * 2. Add testing to tokenLst
 * */
#include "catch.hpp"
#include "commons/token/Token.h"
#include "MockLexer.h"
#include "sp/SyntaxValidator.h"

TEST_CASE("SyntaxValidator can validate very simple program correctly") {
    SECTION("SV can validate blank program") {
        Token eof (Token::Tag::EndOfFile);
        std::vector<Token> tokens = {eof};
        MockLexer lex (tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SyntaxValidator sv (std::move(lexPtr));

        REQUIRE(sv.validateProgram() == true);
    }
    SECTION("SV can validate one empty procedure") {
        Token eof (Token::Tag::EndOfFile);
        Token proc (Token::Tag::Procedure);
        Token procName ("main", Token::Tag::Name);
        Token lBrace (Token::Tag::LBrace);
        Token rBrace (Token::Tag::RBrace);

        std::vector<Token> tokens = {eof, rBrace, lBrace, procName, proc};
        MockLexer lex (tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SyntaxValidator sv (std::move(lexPtr));

        REQUIRE(sv.validateProgram() == true);
    }
    SECTION("SV can validate one procedure with one print") {
        Token eof (Token::Tag::EndOfFile);
        Token proc (Token::Tag::Procedure);
        Token procName ("main", Token::Tag::Name);
        Token lBrace (Token::Tag::LBrace);
        Token rBrace (Token::Tag::RBrace);
        Token semi (Token::Tag::SemiColon);
        Token printStmt (Token::Tag::Print);
        Token varName ("x", Token::Tag::Name);

        std::vector<Token> tokens = {eof, rBrace, semi, varName, printStmt, lBrace, procName, proc};
        MockLexer lex (tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SyntaxValidator sv (std::move(lexPtr));

        REQUIRE(sv.validateProgram() == true);
    }
    SECTION("SV can validate one procedure with one read") {
        Token eof (Token::Tag::EndOfFile);
        Token proc (Token::Tag::Procedure);
        Token procName ("main", Token::Tag::Name);
        Token lBrace (Token::Tag::LBrace);
        Token rBrace (Token::Tag::RBrace);
        Token semi (Token::Tag::SemiColon);
        Token readStmt (Token::Tag::Read);
        Token varName ("x", Token::Tag::Name);

        std::vector<Token> tokens = {eof, rBrace, semi, varName, readStmt, lBrace, procName, proc};
        MockLexer lex (tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SyntaxValidator sv (std::move(lexPtr));

        REQUIRE(sv.validateProgram() == true);
    }
}

TEST_CASE("SV can validate assign statement correctly") {
    SECTION("SV can validate x = 1;") {
        Token eof (Token::Tag::EndOfFile);
        Token proc (Token::Tag::Procedure);
        Token procName ("main", Token::Tag::Name);
        Token lBrace (Token::Tag::LBrace);
        Token rBrace (Token::Tag::RBrace);
        Token semi (Token::Tag::SemiColon);
        Token varName ("x", Token::Tag::Name);
        Token assignment ("=", Token::Tag::Assignment);
        Token constVal ("1", Token::Tag::Integer);

        std::vector<Token> tokens = {eof, rBrace, semi, constVal, assignment, varName, lBrace, procName, proc};
        MockLexer lex (tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SyntaxValidator sv (std::move(lexPtr));

        REQUIRE(sv.validateProgram() == true);
    }

    SECTION("SV can validate x = y;") {
        Token eof (Token::Tag::EndOfFile);
        Token proc (Token::Tag::Procedure);
        Token procName ("main", Token::Tag::Name);
        Token lBrace (Token::Tag::LBrace);
        Token rBrace (Token::Tag::RBrace);
        Token semi (Token::Tag::SemiColon);
        Token varName1 ("x", Token::Tag::Name);
        Token assignment ("=", Token::Tag::Assignment);
        Token varName2 ("y", Token::Tag::Name);

        std::vector<Token> tokens = {eof, rBrace, semi, varName2, assignment, varName1, lBrace, procName, proc};
        MockLexer lex (tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SyntaxValidator sv (std::move(lexPtr));

        REQUIRE(sv.validateProgram() == true);
    }

    SECTION("SV can validate x = x + 1;") {
        Token eof (Token::Tag::EndOfFile);
        Token proc (Token::Tag::Procedure);
        Token procName ("main", Token::Tag::Name);
        Token lBrace (Token::Tag::LBrace);
        Token rBrace (Token::Tag::RBrace);
        Token semi (Token::Tag::SemiColon);
        Token varName1 ("x", Token::Tag::Name);
        Token assignment ("=", Token::Tag::Assignment);
        Token plus ("+", Token::Tag::Plus);
        Token constVal ("1", Token::Tag::Integer);

        std::vector<Token> tokens = {
            eof, rBrace,
            semi, constVal, plus, varName1, assignment, varName1,
            lBrace,procName, proc
        };

        MockLexer lex (tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SyntaxValidator sv (std::move(lexPtr));

        REQUIRE(sv.validateProgram() == true);
    }

    SECTION("SV can validate x = x * 1;") {
        Token eof (Token::Tag::EndOfFile);
        Token proc (Token::Tag::Procedure);
        Token procName ("main", Token::Tag::Name);
        Token lBrace (Token::Tag::LBrace);
        Token rBrace (Token::Tag::RBrace);
        Token semi (Token::Tag::SemiColon);
        Token varName1 ("x", Token::Tag::Name);
        Token assignment ("=", Token::Tag::Assignment);
        Token mul ("*", Token::Tag::Multiply);
        Token constVal ("1", Token::Tag::Integer);

        std::vector<Token> tokens = {
            eof, rBrace,
            semi, constVal, mul, varName1, assignment, varName1,
            lBrace,procName, proc
        };

        MockLexer lex (tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SyntaxValidator sv (std::move(lexPtr));

        REQUIRE(sv.validateProgram() == true);
    }

    SECTION("SV can validate x = ((x + 1) % (y - z) + (2 * 3)) / x;") {
        Token eof (Token::Tag::EndOfFile);
        Token proc (Token::Tag::Procedure);
        Token procName ("main", Token::Tag::Name);
        Token lBrace (Token::Tag::LBrace);
        Token rBrace (Token::Tag::RBrace);
        Token lParen (Token::Tag::LParen);
        Token rParen (Token::Tag::RParen);
        Token semi (Token::Tag::SemiColon);
        Token assignment ("=", Token::Tag::Assignment);
        Token plus ("+", Token::Tag::Plus);
        Token minus ("-", Token::Tag::Minus);
        Token mul ("+", Token::Tag::Multiply);
        Token div ("/", Token::Tag::Divide);
        Token mod ("%", Token::Tag::Modulo);
        Token varName1 ("x", Token::Tag::Name);
        Token varName2 ("y", Token::Tag::Name);
        Token varName3 ("z", Token::Tag::Name);
        Token constVal1 ("1", Token::Tag::Integer);
        Token constVal2 ("2", Token::Tag::Integer);
        Token constVal3 ("3", Token::Tag::Integer);


        std::vector<Token> tokens = {
            eof, rBrace,
            semi, varName1, div, rParen, rParen, constVal3, mul, constVal2, lParen, plus,
            rParen, varName3, minus, varName2, lParen, mod, rParen, constVal1, plus, varName1, lParen, lParen,
            assignment, varName1,
            lBrace,procName, proc
        };

        MockLexer lex (tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SyntaxValidator sv (std::move(lexPtr));

        REQUIRE(sv.validateProgram() == true);
    }
}

TEST_CASE("SV can validate multiple stmt program") {
    SECTION("SV can validate two read stmt") {
        Token eof (Token::Tag::EndOfFile);
        Token proc (Token::Tag::Procedure);
        Token procName ("main", Token::Tag::Name);
        Token lBrace (Token::Tag::LBrace);
        Token rBrace (Token::Tag::RBrace);
        Token semi (Token::Tag::SemiColon);
        Token readStmt (Token::Tag::Read);
        Token varName1 ("x", Token::Tag::Name);
        Token varName2 ("y", Token::Tag::Name);

        std::vector<Token> tokens = {
            eof, rBrace,
            semi, varName2, readStmt,
            semi, varName1, readStmt,
            lBrace, procName, proc
        };
        MockLexer lex (tokens);
        std::unique_ptr<ILexer> lexPtr = std::make_unique<MockLexer>(lex);
        SyntaxValidator sv (std::move(lexPtr));

        REQUIRE(sv.validateProgram() == true);
    }
}