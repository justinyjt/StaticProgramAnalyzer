#include "SuchThatClauseParser.h"
#include "qps/clause/relationship/Modify.h"
#include "qps/pql/StatementNumber.h"

Clause* SuchThatClauseParser::parse(const std::unique_ptr<Lexer> &lexer, std::vector<Synonym> synonyms) {
    std::string keyword = lexer->scan().getLexeme();

    if (keyword != "Modifies") {
        throw std::runtime_error("not modifies clause");
    }

    if (lexer->scan().getTag() != Token::Tag::LParen) {
        throw std::runtime_error("missing left parenthesis");
    }

    Token leftArg = lexer->scan();

    Tok left = makeArg(leftArg, synonyms);

    if (lexer->scan().getTag() != Token::Tag::Comma) {
        throw std::runtime_error("missing comma");
    }

    Token rightArg = lexer->scan();

    Tok right = makeArg(rightArg, synonyms);

    if (lexer->scan().getTag() != Token::Tag::RParen) {
        throw std::runtime_error("missing right parenthesis");
    }

    Modify* m = new Modify(left, right);
    return m;
}

Tok SuchThatClauseParser::makeArg(Token token, std::vector<Synonym> synonyms) {
    if (token.getTag() == Token::Tag::Integer) {
        StatementNumber t(token.getLexeme());
        return t;
    } else if (token.getTag() == Token::Tag::Name) {
        for (auto synonym : synonyms) {
            if (synonym.getValue() == token.getLexeme()) {
                Synonym s(synonym.de, token.getLexeme());
                return s;
            }
        }
        throw std::runtime_error("synonym not declared!");
    } else {
        throw std::runtime_error("invalid argument");
    }
}
