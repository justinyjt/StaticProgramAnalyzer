#include "SuchThatClauseParser.h"
#include "qps/clause/relationship/Modify.h"
#include "qps/pql/StatementNumber.h"

Clause& SuchThatClauseParser::parse(const std::unique_ptr<Lexer> &lexer, std::vector<Synonym> synonyms) {
    std::string keyword = lexer->scan().getLexeme();

    if (keyword != "Modifies") {
        throw std::runtime_error("not modifies clause");
    }

    if (lexer->scan().getTag() != Token::Tag::LParen) {
        throw std::runtime_error("missing left parenthesis");
    }

    Token leftArg = lexer->scan();

    Arg& left = makeArg(leftArg, synonyms);

    if (lexer->scan().getTag() != Token::Tag::Comma) {
        throw std::runtime_error("missing comma");
    }

    Token rightArg = lexer->scan();

    Arg& right = makeArg(rightArg, synonyms);

    if (lexer->scan().getTag() != Token::Tag::RParen) {
        throw std::runtime_error("missing right parenthesis");
    }

    Modify m(static_cast<StmtRef&>(left), static_cast<StmtRef&>(right)); // sus..
    return m;
}

Arg SuchThatClauseParser::makeArg(Token token, std::vector<Synonym> synonyms) {
    if (token.getTag() == Token::Tag::Integer) {
        StatementNumber t(std::stoi(token.getLexeme()));
        return t;
    } else if (token.getTag() == Token::Tag::Name) {
        for (auto synonym : synonyms) {
            if (synonym.ident == token.getLexeme()) {
                Synonym s(synonym.de, token.getLexeme());
                return s;
            }
        }
        throw std::runtime_error("synonym not declared!");
    } else {
        throw std::runtime_error("invalid argument");
    }
}
