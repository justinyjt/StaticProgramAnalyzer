#include <string>
#include "PatternClauseParser.h"
#include "qps/clauses/patterns/pattern_clauses/AssignPattern.h"
#include "qps/entities/Synonym.h"
#include "qps/entities/Expression.h"

Pattern PatternClauseParser::parse(const std::unique_ptr<Lexer> &lexer, std::vector<Synonym> synonyms) {
    std::string pattern_type = lexer->scan().getLexeme();

    if (lexer->scan().getTag() != Token::Tag::LParen) {
        throw std::runtime_error("missing left parenthesis");
    }

    if (lexer->scan().getTag() != Token::Tag::Underscore) {
        throw std::runtime_error("expected underscore");
    }

    Term left(Term::Tag::Wildcard);

    if (lexer->scan().getTag() != Token::Tag::Comma) {
        throw std::runtime_error("missing comma");
    }

    Token rightArg = lexer->scan();
    Term right = makeTerm(rightArg, synonyms);


    if (lexer->scan().getTag() != Token::Tag::RParen) {
        throw std::runtime_error("missing right parenthesis");
    }

    AssignPattern a(left, right);
    return a;
}

Term PatternClauseParser::makeTerm(Token token, std::vector<Synonym> synonyms) {
    if (token.getTag() == Token::Tag::String) {
        Expression e(token.getLexeme());
        return e;
    } else {
        throw std::runtime_error("string expected");
    }
}
