#include <memory>

#include "catch.hpp"
#include "../../TestHelper.h"
#include "commons/lexer/Lexer.h"
#include "commons/token/Token.h"

TEST_CASE("1. Lexer test basic token") {
    Source source = Source("procedure p {x = 1;}");
    KeywordList keyword_list = KeywordList{Keyword("procedure", Token::Tag::Procedure)};
    CharacterList character_list = CharacterList{Character(';', Token::Tag::SemiColon),
                                                 Character('{', Token::Tag::LBrace),
                                                 Character('}', Token::Tag::RBrace),
                                                 Character('=', Token::Tag::Assign)};
    Lexer lexer = Lexer(source, keyword_list, character_list);
    std::unique_ptr<Token> token = lexer.scan();
    requireEqual(*token, Token("procedure", Token::Tag::Procedure, 1));
    token = lexer.scan();
    requireEqual(*token, Token("p", Token::Tag::Name, 1));
    token = lexer.scan();
    requireEqual(*token, Token("{", Token::Tag::LBrace, 1));
    token = lexer.scan();
    requireEqual(*token, Token("x", Token::Tag::Name, 1));
    token = lexer.scan();
    requireEqual(*token, Token("=", Token::Tag::Assign, 1));
    token = lexer.scan();
    requireEqual(*token, Token("1", Token::Tag::Integer, 1));
    token = lexer.scan();
    requireEqual(*token, Token(";", Token::Tag::SemiColon, 1));
    token = lexer.scan();
    requireEqual(*token, Token("}", Token::Tag::RBrace, 1));
    token = lexer.scan();
    requireEqual(*token, Token("", Token::Tag::EndOfFile, 1));
}

TEST_CASE("2. Test new line in token") {
    Source source = Source("procedure p {x = 1;\n    y = 2;  \n   call q;}");
    KeywordList keyword_list = KeywordList{Keyword("procedure", Token::Tag::Procedure),
                                           Keyword("call", Token::Tag::Call)};
    CharacterList character_list = CharacterList{Character(';', Token::Tag::SemiColon),
                                                 Character('{', Token::Tag::LBrace),
                                                 Character('}', Token::Tag::RBrace),
                                                 Character('=', Token::Tag::Assign)};
    Lexer lexer = Lexer(source, keyword_list, character_list);
    std::unique_ptr<Token> token = lexer.scan();
    requireEqual(*token, Token("procedure", Token::Tag::Procedure, 1));
    token = lexer.scan();
    requireEqual(*token, Token("p", Token::Tag::Name, 1));
    token = lexer.scan();
    requireEqual(*token, Token("{", Token::Tag::LBrace, 1));
    token = lexer.scan();
    requireEqual(*token, Token("x", Token::Tag::Name, 1));
    token = lexer.scan();
    requireEqual(*token, Token("=", Token::Tag::Assign, 1));
    token = lexer.scan();
    requireEqual(*token, Token("1", Token::Tag::Integer, 1));
    token = lexer.scan();
    requireEqual(*token, Token(";", Token::Tag::SemiColon, 1));
    token = lexer.scan();
    requireEqual(*token, Token("y", Token::Tag::Name, 2));
    token = lexer.scan();
    requireEqual(*token, Token("=", Token::Tag::Assign, 2));
    token = lexer.scan();
    requireEqual(*token, Token("2", Token::Tag::Integer, 2));
    token = lexer.scan();
    requireEqual(*token, Token(";", Token::Tag::SemiColon, 2));
    token = lexer.scan();
    requireEqual(*token, Token("call", Token::Tag::Call, 3));
    token = lexer.scan();
    requireEqual(*token, Token("q", Token::Tag::Name, 3));
    token = lexer.scan();
    requireEqual(*token, Token(";", Token::Tag::SemiColon, 3));
    token = lexer.scan();
    requireEqual(*token, Token("}", Token::Tag::RBrace, 3));
    token = lexer.scan();
    requireEqual(*token, Token("", Token::Tag::EndOfFile, 3));
}
