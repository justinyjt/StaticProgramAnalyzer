#include <string>
#include "PatternClauseParser.h"
#include "qps/clause/pattern/Pattern.h"
#include "qps/pql/Synonym.h"
#include "qps/pql/Wildcard.h"
#include "qps/pql/ExpressionStr.h"

Clause *PatternClauseParser::parse(TokenValidator &tokenValidator, std::vector<Synonym> synonyms) {
    std::string pattern_type = tokenValidator.getNextToken()->getLexeme();

    if (tokenValidator.getNextToken()->getTag() != Token::Tag::LParen) {
        throw std::runtime_error("missing left parenthesis");
    }

    if (tokenValidator.getNextToken()->getTag() != Token::Tag::Underscore) {
        throw std::runtime_error("expected underscore");
    }

    Wildcard left;

    if (tokenValidator.getNextToken()->getTag() != Token::Tag::Comma) {
        throw std::runtime_error("missing comma");
    }

    std::unique_ptr<Token> rightArg = tokenValidator.getNextToken();

    if (rightArg->getTag() != Token::Tag::String) {
        throw std::runtime_error("string expected");
    }

    ExpressionStr right = ExpressionStr(rightArg->getLexeme());

    if (tokenValidator.getNextToken()->getTag() != Token::Tag::RParen) {
        throw std::runtime_error("missing right parenthesis");
    }

    Pattern *p = new Pattern(left, right);
    return p;
}
