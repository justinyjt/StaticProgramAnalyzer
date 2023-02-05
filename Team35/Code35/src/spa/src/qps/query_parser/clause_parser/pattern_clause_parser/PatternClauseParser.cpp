#include <string>
#include "PatternClauseParser.h"
#include "qps/clauses/patterns/pattern_clauses/AssignPattern.h"
#include "qps/entities/Synonym.h"
#include "qps/entities/Expression.h"

Pattern PatternClauseParser::parse(std::unique_ptr<Lexer> lexer, std::vector<std::string> synonyms) {
    std::string pattern_type = lexer->Scan().GetLexeme();

    if (lexer->Scan().GetTag() != Token::Tag::LParen) {
        throw std::runtime_error("missing left parenthesis");
    }

    if (lexer->Scan().GetTag() != Token::Tag::Underscore) {
        throw std::runtime_error("expected underscore");
    }

    Term left(Term::Tag::Wildcard);

    if (lexer->Scan().GetTag() != Token::Tag::Comma) {
        throw std::runtime_error("missing comma");
    }

    Token rightArg = lexer->Scan().GetTag();
    Term right = makeTerm(rightArg, synonyms);


    if (lexer.Scan().GetTag() != Token::Tag::RParen) {
        throw std::runtime_error("missing right parenthesis");
    }

    AssignPattern a(left, right);
    return a;
}

Term PatternClauseParser::makeTerm(Token token, std::vector<Synonym> synonyms) {
    if (token == Token::Tag::String) {
        Expression e(token.GetLexeme());
        return e;
    } else {
        throw std::runtime_error("string expected");
    }
}