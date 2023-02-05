#include "SuchThatClauseParser.h"
#include "qps/clauses/relationships/relationship_clauses/Modify.h"
#include "qps/entities/StatementNumber.h"

Relationship SuchThatClauseParser::parse(const std::unique_ptr<Lexer> &lexer, std::vector<Synonym> synonyms) {
    std::string keyword = lexer->scan().getLexeme();

    if (keyword != "Modifies") {
        throw std::runtime_error("not modifies clause");
    }

    if (lexer->scan().getTag() != Token::Tag::LParen) {
        throw std::runtime_error("missing left parenthesis");
    }

    Token leftArg = lexer->scan();

    Term left = makeTerm(leftArg, synonyms);

    if (lexer->scan().getTag() != Token::Tag::Comma) {
        throw std::runtime_error("missing comma");
    }

    Token rightArg = lexer->scan();

    Term right = makeTerm(rightArg, synonyms);

    if (lexer->scan().getTag() != Token::Tag::RParen) {
        throw std::runtime_error("missing right parenthesis");
    }

    Modify m(left, right);
    return m;
}

Term SuchThatClauseParser::makeTerm(Token token, std::vector<Synonym> synonyms) {
    if (token.getTag() == Token::Tag::Integer) {
        StatementNumber t(std::stoi(token.getLexeme()));
        return t;
    } else if (token.getTag() == Token::Tag::Name) {
        for (auto synonym : synonyms) {
            if (synonym.getDeclaration() == token.getLexeme()) {
                Synonym s(synonym.getDesignEntity(), token.getLexeme());
                return s;
            }
        }
        throw std::runtime_error("synonym not declared!");
    } else {
        throw std::runtime_error("invalid argument");
    }
}
