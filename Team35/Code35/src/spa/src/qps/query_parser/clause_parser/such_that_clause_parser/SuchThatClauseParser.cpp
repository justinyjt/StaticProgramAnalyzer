#include "SuchThatClauseParser.h"
#include "qps/clauses/relationships/relationship_clauses/Modify.h"
#include "qps/entities/StatementNumber.h"

Relationship SuchThatClauseParser::parse(std::unique_ptr<Lexer> lexer, std::vector<Synonym> synonyms) {

    std::string keyword = lexer->Scan().GetLexeme();

    if (keyword != "Modifies") {
        throw std::runtime_error("not modifies clause");
    }

    if (lexer->Scan().GetTag() != Token::Tag::LParen) {
        throw std::runtime_error("missing left parenthesis");
    }

    Token leftArg = lexer->Scan();

    Term left = makeTerm(leftArg, synonyms);

    if (lexer->Scan().GetTag() != Token::Tag::Comma) {
        throw std::runtime_error("missing comma");
    }

    Token::Token rightArg = lexer->Scan();

    Term right = makeTerm(rightArg, synonyms);

    if (lexer->Scan().GetTag() != Token::Tag::RParen) {
        throw std::runtime_error("missing right parenthesis");
    }

    Modify m(left, right);
    return m;
}

Term SuchThatClauseParser::makeTerm(Token token, std::vector<Synonym> synonyms) {
    if (token == Token::Tag::Integer) {
        StatementNumber t(token.GetInteger());
        return t;
    } else if (token == Token::Tag::Name) {
        for (auto synonym : synonyms) {
            if (synonym.getDeclaration() == token.GetLexeme()) {
                Synonym t(token.GetLexeme());
                return t;
            }
        }
        throw std::runtime_error("synonym not declared!");
    } else {
        throw std::runtime_error("invalid argument");
    }
}
