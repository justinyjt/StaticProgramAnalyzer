#include "LexerFactory.h"

#include <cassert>
#include <utility>
#include <set>

#include "Lexer.h"

std::unique_ptr<ILexer> LexerFactory::CreateLexer(Source source, LexerFactory::LexerType lexer_type) {
    switch (lexer_type) {
        case LexerFactory::LexerType::Simple:return std::move(CreateSimpleLexer(source));
        case LexerFactory::LexerType::Pql:return std::move(CreatePqlLexer(source));
        case LexerFactory::LexerType::Expression:return std::move(CreateExpressionLexer(source));
        default:assert(false);
    }
}

std::unique_ptr<ILexer> LexerFactory::CreateSimpleLexer(Source source) {
    KeywordList keyword_list{
        Keyword("procedure", Token::Tag::Procedure),
        Keyword("while", Token::Tag::While),
        Keyword("if", Token::Tag::If),
        Keyword("then", Token::Tag::Then),
        Keyword("else", Token::Tag::Else),
        Keyword("call", Token::Tag::Call),
        Keyword("read", Token::Tag::Read),
        Keyword("print", Token::Tag::Print),
    };
    CharacterList character_list{
        Character('(', Token::Tag::LParen),
        Character(')', Token::Tag::RParen),
        Character('{', Token::Tag::LBrace),
        Character('}', Token::Tag::RBrace),
        Character(';', Token::Tag::SemiColon),
        Character(',', Token::Tag::Comma),
        Character('+', Token::Tag::Plus),
        Character('-', Token::Tag::Minus),
        Character('*', Token::Tag::Multiply),
        Character('/', Token::Tag::Divide),
        Character('%', Token::Tag::Modulo),
        Character('=', Token::Tag::Assignment),
    };
    std::unique_ptr<ILexer> lexer = std::make_unique<Lexer>(Lexer(std::move(source), keyword_list, character_list));
    return std::move(lexer);
}

std::unique_ptr<ILexer> LexerFactory::CreatePqlLexer(Source source) {
    KeywordList keyword_list{
        Keyword("procedure", Token::Tag::Procedure),
        Keyword("call", Token::Tag::Call),
        Keyword("read", Token::Tag::Read),
        Keyword("print", Token::Tag::Print),
        Keyword("call", Token::Tag::Call),
        Keyword("variable", Token::Tag::Variable),
        Keyword("constant", Token::Tag::Constant),
        Keyword("stmt", Token::Tag::Statement),
        Keyword("assign", Token::Tag::Assign),
        Keyword("while", Token::Tag::While),
        Keyword("if", Token::Tag::If),
        Keyword("call", Token::Tag::Call),
        Keyword("Select", Token::Tag::Select),
        Keyword("Follows", Token::Tag::Follows),
        Keyword("Modifies", Token::Tag::Modifies),
        Keyword("Uses", Token::Tag::Uses),
        Keyword("Parent", Token::Tag::Parent),
        Keyword("pattern", Token::Tag::Pattern),
        Keyword("such", Token::Tag::Such),
        Keyword("that", Token::Tag::That),
    };
    CharacterList character_list{
        Character('(', Token::Tag::LParen),
        Character(')', Token::Tag::RParen),
        Character(';', Token::Tag::SemiColon),
        Character(',', Token::Tag::Comma),
        Character('_', Token::Tag::Underscore),
    };
    std::unique_ptr<ILexer>
        lexer = std::make_unique<Lexer>(Lexer(std::move(source), keyword_list, character_list, false));
    return std::move(lexer);
}

std::unique_ptr<ILexer> LexerFactory::CreateExpressionLexer(Source source) {
    KeywordList keyword_list{};
    CharacterList character_list{
        Character('(', Token::Tag::LParen),
        Character(')', Token::Tag::RParen),
        Character('+', Token::Tag::Plus),
        Character('-', Token::Tag::Minus),
        Character('*', Token::Tag::Multiply),
        Character('/', Token::Tag::Divide),
        Character('%', Token::Tag::Modulo),
        Character('=', Token::Tag::Assignment),
    };
    std::unique_ptr<ILexer>
        lexer = std::make_unique<Lexer>(Lexer(std::move(source), keyword_list, character_list, false));
    return std::move(lexer);
}
