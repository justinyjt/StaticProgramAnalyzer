#include <string>
#include "PatternClauseParser.h"
#include "qps/clause/pattern/Pattern.h"
#include "qps/pql/Synonym.h"
#include "qps/pql/Wildcard.h"
#include "qps/pql/ExpressionStr.h"

Clause* PatternClauseParser::parse(const std::unique_ptr<Lexer> &lexer, std::vector<Synonym> synonyms) {
    std::string pattern_type = lexer->scan().getLexeme();

    if (lexer->scan().getTag() != Token::Tag::LParen) {
        throw std::runtime_error("missing left parenthesis");
    }

    if (lexer->scan().getTag() != Token::Tag::Underscore) {
        throw std::runtime_error("expected underscore");
    }

    Wildcard left;

    if (lexer->scan().getTag() != Token::Tag::Comma) {
        throw std::runtime_error("missing comma");
    }

    Token rightArg = lexer->scan();

    if (rightArg.getTag() != Token::Tag::String) {
        throw std::runtime_error("string expected");
    }

    ExpressionStr right = ExpressionStr(rightArg.getLexeme());

    if (lexer->scan().getTag() != Token::Tag::RParen) {
        throw std::runtime_error("missing right parenthesis");
    }

    Pattern* p = new Pattern(left, right);
    return p;
}
