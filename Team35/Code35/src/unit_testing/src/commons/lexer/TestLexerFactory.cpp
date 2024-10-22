#include <memory>

#include "catch.hpp"
#include "../../TestHelper.h"
#include "commons/lexer/ILexer.h"
#include "commons/lexer/LexerFactory.h"
#include "commons/token/Token.h"

TEST_CASE("1. test lexer factory for pql") {
    std::unique_ptr<ILexer>
        lexer = LexerFactory::createLexer("Select s such that Modifies(1, v)\n Select a pattern a ( _ , \"x + 1\")",
                                          LexerFactory::LexerType::Pql);
    std::unique_ptr<Token> token = lexer->scan();
    requireEqual(*token, Token("Select", Token::Tag::Select));
    token = lexer->scan();
    requireEqual(*token, Token("s", Token::Tag::Name));
    token = lexer->scan();
    requireEqual(*token, Token("such", Token::Tag::Such));
    token = lexer->scan();
    requireEqual(*token, Token("that", Token::Tag::That));
    token = lexer->scan();
    requireEqual(*token, Token("Modifies", Token::Tag::Modifies));
    token = lexer->scan();
    requireEqual(*token, Token("(", Token::Tag::LParen));
    token = lexer->scan();
    requireEqual(*token, Token("1", Token::Tag::Integer));
    token = lexer->scan();
    requireEqual(*token, Token(",", Token::Tag::Comma));
    token = lexer->scan();
    requireEqual(*token, Token("v", Token::Tag::Name));
    token = lexer->scan();
    requireEqual(*token, Token(")", Token::Tag::RParen));
    token = lexer->scan();
    requireEqual(*token, Token("Select", Token::Tag::Select));
    token = lexer->scan();
    requireEqual(*token, Token("a", Token::Tag::Name));
    token = lexer->scan();
    requireEqual(*token, Token("pattern", Token::Tag::Pattern));
    token = lexer->scan();
    requireEqual(*token, Token("a", Token::Tag::Name));
    token = lexer->scan();
    requireEqual(*token, Token("(", Token::Tag::LParen));
    token = lexer->scan();
    requireEqual(*token, Token("_", Token::Tag::Underscore));
    token = lexer->scan();
    requireEqual(*token, Token(",", Token::Tag::Comma));
    token = lexer->scan();
    requireEqual(*token, Token("x + 1", Token::Tag::String));
    token = lexer->scan();
    requireEqual(*token, Token(")", Token::Tag::RParen));
    token = lexer->scan();
    requireEqual(*token, Token("", Token::Tag::EndOfFile));
}

TEST_CASE("2. test lexer factory for simple") {
    std::unique_ptr<ILexer>
        lexer = LexerFactory::createLexer("procedure main { x = 1; }", LexerFactory::LexerType::Simple);
    std::unique_ptr<Token> token = lexer->scan();
    requireEqual(*token, Token("procedure", Token::Tag::Procedure));
    token = lexer->scan();
    requireEqual(*token, Token("main", Token::Tag::Name));
    token = lexer->scan();
    requireEqual(*token, Token("{", Token::Tag::LBrace));
    token = lexer->scan();
    requireEqual(*token, Token("x", Token::Tag::Name));
    token = lexer->scan();
    requireEqual(*token, Token("=", Token::Tag::Assignment));
    token = lexer->scan();
    requireEqual(*token, Token("1", Token::Tag::Integer));
    token = lexer->scan();
    requireEqual(*token, Token(";", Token::Tag::SemiColon));
    token = lexer->scan();
    requireEqual(*token, Token("}", Token::Tag::RBrace));
    token = lexer->scan();
    requireEqual(*token, Token("", Token::Tag::EndOfFile));
}

TEST_CASE("3. test lexer factory for expression") {
    std::unique_ptr<ILexer>
        lexer = LexerFactory::createLexer("x = v + x * y + z * t", LexerFactory::LexerType::Expression);
    std::unique_ptr<Token> token = lexer->scan();
    requireEqual(*token, Token("x", Token::Tag::Name));
    token = lexer->scan();
    requireEqual(*token, Token("=", Token::Tag::Assignment));
    token = lexer->scan();
    requireEqual(*token, Token("v", Token::Tag::Name));
    token = lexer->scan();
    requireEqual(*token, Token("+", Token::Tag::Plus));
    token = lexer->scan();
    requireEqual(*token, Token("x", Token::Tag::Name));
    token = lexer->scan();
    requireEqual(*token, Token("*", Token::Tag::Multiply));
    token = lexer->scan();
    requireEqual(*token, Token("y", Token::Tag::Name));
    token = lexer->scan();
    requireEqual(*token, Token("+", Token::Tag::Plus));
    token = lexer->scan();
    requireEqual(*token, Token("z", Token::Tag::Name));
    token = lexer->scan();
    requireEqual(*token, Token("*", Token::Tag::Multiply));
    token = lexer->scan();
    requireEqual(*token, Token("t", Token::Tag::Name));
    token = lexer->scan();
    requireEqual(*token, Token("", Token::Tag::EndOfFile));
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
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(source, LexerFactory::LexerType::Simple);
    std::unique_ptr<Token> token = lexer->scan();
    for (int i = 0; i < 21; i++) {
        token = lexer->scan();
    }
    requireEqual(*token, Token("{", Token::Tag::LBrace));
    for (int i = 0; i < 10; i++) {
        token = lexer->scan();
    }
    requireEqual(*token, Token("==", Token::Tag::Equivalence));
    requireNotThrow([&token, &lexer]() {
      while (token->getTag() != Token::Tag::EndOfFile) {
          token = lexer->scan();
      }
    });
}
