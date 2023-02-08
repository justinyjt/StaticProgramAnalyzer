#include "SuchThatClauseParser.h"
#include "qps/clause/relationship/Modify.h"
#include "qps/pql/StatementNumber.h"

Clause *SuchThatClauseParser::parse(const std::unique_ptr<ILexer> &lexer, std::vector<Synonym> synonyms) {
    std::string keyword = lexer->scan()->getLexeme();

    if (keyword != "Modifies") {
        throw std::runtime_error("not modifies clause");
    }

    if (lexer->scan()->getTag() != Token::Tag::LParen) {
        throw std::runtime_error("missing left parenthesis");
    }

    std::unique_ptr<Token> leftArg = lexer->scan();

    Tok left = makeArg(std::move(leftArg), synonyms);

    if (lexer->scan()->getTag() != Token::Tag::Comma) {
        throw std::runtime_error("missing comma");
    }

    std::unique_ptr<Token> rightArg = lexer->scan();

    Tok right = makeArg(std::move(rightArg), synonyms);

    if (lexer->scan()->getTag() != Token::Tag::RParen) {
        throw std::runtime_error("missing right parenthesis");
    }

    Modify *m = new Modify(left, right);
    return m;
}

Tok SuchThatClauseParser::makeArg(std::unique_ptr<Token> token, std::vector<Synonym> synonyms) {
    if (token->getTag() == Token::Tag::Integer) {
        StatementNumber t(token->getLexeme());
        return t;
    } else if (token->getTag() == Token::Tag::Name) {
        for (auto synonym : synonyms) {
            if (synonym.getValue() == token->getLexeme()) {
                Synonym s(synonym.de, token->getLexeme());
                return s;
            }
        }
        throw std::runtime_error("synonym not declared!");
    } else {
        throw std::runtime_error("invalid argument");
    }
}
