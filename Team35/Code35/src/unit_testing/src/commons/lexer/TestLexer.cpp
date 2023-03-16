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

TEST_CASE("3. test == / != operator of Lexer") {
    Source source = Source("procedure p {x = 1;}");
    KeywordList keyword_list = KeywordList{Keyword("procedure", Token::Tag::Procedure)};
    KeywordList keyword_list2 = KeywordList{Keyword("procedure", Token::Tag::Procedure),
                                            Keyword("call", Token::Tag::Call)};
    CharacterList character_list = CharacterList{Character(';', Token::Tag::SemiColon),
                                                 Character('{', Token::Tag::LBrace),
                                                 Character('}', Token::Tag::RBrace),
                                                 Character('=', Token::Tag::Assign)};
    Lexer lexer = Lexer(source, keyword_list, character_list);
    Lexer lexer2 = Lexer(source, keyword_list, character_list);
    Lexer lexer3 = Lexer(source, keyword_list, character_list, false);
    Lexer lexer4 = Lexer(source, keyword_list2, character_list);
    requireEqualRef(lexer, lexer2);
    requireNotEqualRef(lexer, lexer3);
    requireNotEqualRef(lexer, lexer4);
    requireNotEqualRef(lexer3, lexer4);
}

TEST_CASE("4. test Builder class of Lexer") {
    SECTION("4.1. all parameters are set") {
        Source source = Source("procedure p {x = 1;}");
        KeywordList keyword_list = KeywordList{Keyword("procedure", Token::Tag::Procedure)};
        CharacterList character_list = CharacterList{Character(';', Token::Tag::SemiColon),
                                                     Character('{', Token::Tag::LBrace),
                                                     Character('}', Token::Tag::RBrace),
                                                     Character('=', Token::Tag::Assign)};
        std::unique_ptr<Lexer> lxr = Lexer::Builder()
            .setSource(source)
            .addKeyword(Keyword("procedure", Token::Tag::Procedure))
            .addCharacter(Character(';', Token::Tag::SemiColon))
            .addCharacter(Character('{', Token::Tag::LBrace))
            .addCharacter(Character('}', Token::Tag::RBrace))
            .addCharacter(Character('=', Token::Tag::Assign))
            .setIncludeOperator(false)
            .setIncludeString(false)
            .build();
        Lexer lxr2 = Lexer(source, keyword_list, character_list, false, false);
        requireEqualRef(*lxr, lxr2);
    }

    SECTION("4.2. some parameters are not set") {
        Source source = Source("procedure p {x = 1;}");
        KeywordList keyword_list = KeywordList{Keyword("procedure", Token::Tag::Procedure)};
        CharacterList character_list = CharacterList{Character(';', Token::Tag::SemiColon),
                                                     Character('{', Token::Tag::LBrace),
                                                     Character('}', Token::Tag::RBrace),
                                                     Character('=', Token::Tag::Assign)};
        std::unique_ptr<Lexer> lxr = Lexer::Builder()
            .setSource(source)
            .addKeyword(Keyword("procedure", Token::Tag::Procedure))
            .addCharacter(Character(';', Token::Tag::SemiColon))
            .addCharacter(Character('{', Token::Tag::LBrace))
            .addCharacter(Character('}', Token::Tag::RBrace))
            .addCharacter(Character('=', Token::Tag::Assign))
            .build();
        Lexer lxr2 = Lexer(source, keyword_list, character_list);
        requireEqualRef(*lxr, lxr2);
    }

    SECTION("4.3. no parameters are set") {
        std::unique_ptr<Lexer> lxr = Lexer::Builder().build();
        Lexer lxr2 = Lexer("", KeywordList{}, CharacterList{});
        requireEqualRef(*lxr, lxr2);
    }

    SECTION("4.4. setSource() is not called") {
        std::unique_ptr<Lexer> lxr = Lexer::Builder()
            .addKeyword(Keyword("procedure", Token::Tag::Procedure))
            .addCharacter(Character(';', Token::Tag::SemiColon))
            .addCharacter(Character('{', Token::Tag::LBrace))
            .addCharacter(Character('}', Token::Tag::RBrace))
            .addCharacter(Character('=', Token::Tag::Assign))
            .build();
        Lexer lxr2 = Lexer("", KeywordList{Keyword("procedure", Token::Tag::Procedure)},
                           CharacterList{Character(';', Token::Tag::SemiColon),
                                         Character('{', Token::Tag::LBrace),
                                         Character('}', Token::Tag::RBrace),
                                         Character('=', Token::Tag::Assign)});
        requireEqualRef(*lxr, lxr2);
    }

    SECTION("4.5. addKeyword() is not called") {
        Source source = Source("procedure p {x = 1;}");
        CharacterList character_list = CharacterList{Character(';', Token::Tag::SemiColon),
                                                     Character('{', Token::Tag::LBrace),
                                                     Character('}', Token::Tag::RBrace),
                                                     Character('=', Token::Tag::Assign)};
        std::unique_ptr<Lexer> lxr = Lexer::Builder()
            .setSource(source)
            .addCharacter(Character(';', Token::Tag::SemiColon))
            .addCharacter(Character('{', Token::Tag::LBrace))
            .addCharacter(Character('}', Token::Tag::RBrace))
            .addCharacter(Character('=', Token::Tag::Assign))
            .build();
        Lexer lxr2 = Lexer(source, KeywordList{}, character_list);
        requireEqualRef(*lxr, lxr2);
    }

    SECTION("4.6. addCharacter() is not called") {
        Source source = Source("procedure p {x = 1;}");
        KeywordList keyword_list = KeywordList{Keyword("procedure", Token::Tag::Procedure)};
        std::unique_ptr<Lexer> lxr = Lexer::Builder()
            .setSource(source)
            .addKeyword(Keyword("procedure", Token::Tag::Procedure))
            .build();
        Lexer lxr2 = Lexer(source, keyword_list, CharacterList{});
        requireEqualRef(*lxr, lxr2);
    }
}
