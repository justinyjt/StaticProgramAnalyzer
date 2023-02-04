#include <memory>

#include "catch.hpp"
#include "../../TestHelper.h"
#include "commons/lexer/ILexer.h"
#include "commons/lexer/LexerFactory.h"
#include "commons/token/Token.h"

TEST_CASE("1. test lexer factory for pql") {
    std::unique_ptr<ILexer>
        lexer = LexerFactory::CreateLexer("Select s such that Modifies(1, v)\n Select a pattern a ( _ , \"x + 1\")",
                                          LexerFactory::LexerType::Pql);
    Token token = lexer->Scan();
    RequireEqual(token, Token("Select", Token::Tag::Select, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("s", Token::Tag::Name, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("such", Token::Tag::Such, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("that", Token::Tag::That, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("Modifies", Token::Tag::Modifies, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("(", Token::Tag::LParen, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("1", Token::Tag::Integer, 1));
    token = lexer->Scan();
    RequireEqual(token, Token(",", Token::Tag::Comma, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("v", Token::Tag::Name, 1));
    token = lexer->Scan();
    RequireEqual(token, Token(")", Token::Tag::RParen, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("Select", Token::Tag::Select, 2));
    token = lexer->Scan();
    RequireEqual(token, Token("a", Token::Tag::Name, 2));
    token = lexer->Scan();
    RequireEqual(token, Token("pattern", Token::Tag::Pattern, 2));
    token = lexer->Scan();
    RequireEqual(token, Token("a", Token::Tag::Name, 2));
    token = lexer->Scan();
    RequireEqual(token, Token("(", Token::Tag::LParen, 2));
    token = lexer->Scan();
    RequireEqual(token, Token("_", Token::Tag::Underscore, 2));
    token = lexer->Scan();
    RequireEqual(token, Token(",", Token::Tag::Comma, 2));
    token = lexer->Scan();
    RequireEqual(token, Token("x + 1", Token::Tag::String, 2));
    token = lexer->Scan();
    RequireEqual(token, Token(")", Token::Tag::RParen, 2));
    token = lexer->Scan();
    RequireEqual(token, Token("", Token::Tag::EndOfFile, 2));
}

TEST_CASE("2. test lexer factory for simple") {
    std::unique_ptr<ILexer>
        lexer = LexerFactory::CreateLexer("procedure main { x = 1; }", LexerFactory::LexerType::Simple);
    Token token = lexer->Scan();
    RequireEqual(token, Token("procedure", Token::Tag::Procedure, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("main", Token::Tag::Name, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("{", Token::Tag::LBrace, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("x", Token::Tag::Name, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("=", Token::Tag::Assignment, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("1", Token::Tag::Integer, 1));
    token = lexer->Scan();
    RequireEqual(token, Token(";", Token::Tag::SemiColon, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("}", Token::Tag::RBrace, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("", Token::Tag::EndOfFile, 1));
}

TEST_CASE("3. test lexer factory for expression") {
    std::unique_ptr<ILexer>
        lexer = LexerFactory::CreateLexer("x = v + x * y + z * t", LexerFactory::LexerType::Expression);
    Token token = lexer->Scan();
    RequireEqual(token, Token("x", Token::Tag::Name, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("=", Token::Tag::Assignment, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("v", Token::Tag::Name, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("+", Token::Tag::Plus, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("x", Token::Tag::Name, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("*", Token::Tag::Multiply, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("y", Token::Tag::Name, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("+", Token::Tag::Plus, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("z", Token::Tag::Name, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("*", Token::Tag::Multiply, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("t", Token::Tag::Name, 1));
    token = lexer->Scan();
    RequireEqual(token, Token("", Token::Tag::EndOfFile, 1));
}

TEST_CASE("4. Rigorous test for lexer factory - simple") {
    Source source("procedure Example {\n"
                  "  x = 2;\n"
                  "  z = 3;\n"
                  "  i = 5;\n"
                  "  while (i!=0) {\n"
                  "    x = x - 1;\n"
                  "    if (x==1) then {\n"
                  "      z = x + 1; }\n"
                  "    else {\n"
                  "      y = z + x; }\n"
                  "    z = z + x + i;\n"
                  "    call q;\n"
                  "    i = i - 1; }\n"
                  "  call p; }\n"
                  "\n"
                  "procedure p {\n"
                  "  if (x<0) then {\n"
                  "    while (i>0) {\n"
                  "      x = z * 3 + 2 * y;\n"
                  "      call q;\n"
                  "      i = i - 1; }\n"
                  "    x = x + 1;\n"
                  "    z = x + z; }\n"
                  "  else {\n"
                  "    z = 1; }\n"
                  "  z = z + x + i; }\n"
                  "\n"
                  "procedure q {\n"
                  "  if (x==1) then {\n"
                  "    z = x + 1; }\n"
                  "  else {\n"
                  "    x = z + x; } }");
    std::unique_ptr<ILexer> lexer = LexerFactory::CreateLexer(source, LexerFactory::LexerType::Simple);
    Token token = lexer->Scan();
    for (int i = 0; i < 21; i++) {
        token = lexer->Scan();
    }
    RequireEqual(token, Token("{", Token::Tag::LBrace, 5));
    for (int i = 0; i < 10; i++) {
        token = lexer->Scan();
    }
    RequireEqual(token, Token("==", Token::Tag::Equivalence, 7));
    RequireNotThrow([&token, &lexer]() {
      while (token.GetTag() != Token::Tag::EndOfFile) {
          token = lexer->Scan();
      }
    });
}