#include "LexerFactory.h"

#include <cassert>
#include <utility>

#include "Lexer.h"

std::unique_ptr<ILexer> LexerFactory::createLexer(const Source &source, LexerFactory::LexerType lexer_type) {
    switch (lexer_type) {
        case LexerFactory::LexerType::Simple:
            return std::move(createSimpleLexer(source));
        case LexerFactory::LexerType::Pql:
            return std::move(createPqlLexer(source));
        case LexerFactory::LexerType::Expression:
            return std::move(createExpressionLexer(source));
        default:
            assert(false);
    }
}

std::unique_ptr<ILexer> LexerFactory::createSimpleLexer(const Source &source) {
    Lexer::Builder builder;
    builder.setSource(source).setIncludeOperator(true).setIncludeString(true)
        .addKeyword(Keyword("procedure", Token::Tag::Procedure))
        .addKeyword(Keyword("while", Token::Tag::While))
        .addKeyword(Keyword("if", Token::Tag::If))
        .addKeyword(Keyword("then", Token::Tag::Then))
        .addKeyword(Keyword("else", Token::Tag::Else))
        .addKeyword(Keyword("call", Token::Tag::Call))
        .addKeyword(Keyword("read", Token::Tag::Read))
        .addKeyword(Keyword("print", Token::Tag::Print))
        .addCharacter(Character('(', Token::Tag::LParen))
        .addCharacter(Character(')', Token::Tag::RParen))
        .addCharacter(Character('{', Token::Tag::LBrace))
        .addCharacter(Character('}', Token::Tag::RBrace))
        .addCharacter(Character(';', Token::Tag::SemiColon))
        .addCharacter(Character('+', Token::Tag::Plus))
        .addCharacter(Character('-', Token::Tag::Minus))
        .addCharacter(Character('*', Token::Tag::Multiply))
        .addCharacter(Character('/', Token::Tag::Divide))
        .addCharacter(Character('%', Token::Tag::Modulo))
        .addCharacter(Character('=', Token::Tag::Assignment));
    return builder.build();
}

std::unique_ptr<ILexer> LexerFactory::createPqlLexer(const Source &source) {
    Lexer::Builder builder;
    builder.setSource(source).setIncludeOperator(false).setIncludeString(true)
        .addKeyword(Keyword("procedure", Token::Tag::Procedure))
        .addKeyword(Keyword("call", Token::Tag::Call))
        .addKeyword(Keyword("read", Token::Tag::Read))
        .addKeyword(Keyword("print", Token::Tag::Print))
        .addKeyword(Keyword("call", Token::Tag::Call))
        .addKeyword(Keyword("variable", Token::Tag::Variable))
        .addKeyword(Keyword("constant", Token::Tag::Constant))
        .addKeyword(Keyword("stmt", Token::Tag::Statement))
        .addKeyword(Keyword("assign", Token::Tag::Assign))
        .addKeyword(Keyword("while", Token::Tag::While))
        .addKeyword(Keyword("if", Token::Tag::If))
        .addKeyword(Keyword("call", Token::Tag::Call))
        .addKeyword(Keyword("Select", Token::Tag::Select))
        .addKeyword(Keyword("BOOLEAN", Token::Tag::Bool))
        .addKeyword(Keyword("Follows", Token::Tag::Follows))
        .addKeyword(Keyword("Modifies", Token::Tag::Modifies))
        .addKeyword(Keyword("Uses", Token::Tag::Uses))
        .addKeyword(Keyword("Parent", Token::Tag::Parent))
        .addKeyword(Keyword("pattern", Token::Tag::Pattern))
        .addKeyword(Keyword("such", Token::Tag::Such))
        .addKeyword(Keyword("that", Token::Tag::That))
        .addKeyword(Keyword("Calls", Token::Tag::Calls))
        .addKeyword(Keyword("with", Token::Tag::With))
        .addKeyword(Keyword("and", Token::Tag::And))
        .addCharacter(Character('*', Token::Tag::Star))
        .addCharacter(Character('(', Token::Tag::LParen))
        .addCharacter(Character(')', Token::Tag::RParen))
        .addCharacter(Character(';', Token::Tag::SemiColon))
        .addCharacter(Character(',', Token::Tag::Comma))
        .addCharacter(Character('_', Token::Tag::Underscore))
        .addCharacter(Character('=', Token::Tag::Equal))
        .addCharacter(Character('.', Token::Tag::Dot))
        .addCharacter(Character('#', Token::Tag::Hex))
        .addCharacter(Character('<', Token::Tag::LessThan))
        .addCharacter(Character('>', Token::Tag::GreaterThan));
    return builder.build();
}

std::unique_ptr<ILexer> LexerFactory::createExpressionLexer(const Source &source) {
    Lexer::Builder builder;
    builder.setSource(source).setIncludeOperator(false).setIncludeString(false)
        .addCharacter(Character('(', Token::Tag::LParen))
        .addCharacter(Character(')', Token::Tag::RParen))
        .addCharacter(Character('+', Token::Tag::Plus))
        .addCharacter(Character('-', Token::Tag::Minus))
        .addCharacter(Character('*', Token::Tag::Multiply))
        .addCharacter(Character('/', Token::Tag::Divide))
        .addCharacter(Character('%', Token::Tag::Modulo))
        .addCharacter(Character('=', Token::Tag::Assignment));
    return builder.build();
}
