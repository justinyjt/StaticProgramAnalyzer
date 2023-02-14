#include <string>
#include "PatternClauseParser.h"
#include "qps/clause/pattern/Pattern.h"
#include "qps/pql/Synonym.h"
#include "qps/pql/Wildcard.h"
#include "qps/pql/ExpressionStr.h"
#include "qps/pql/StatementNumber.h"
#include "qps/pql/IdentStr.h"
#include "qps/query_exceptions/SyntaxException.h"

Clause *PatternClauseParser::parse(TokenValidator &tokenValidator, std::vector<Synonym> synonyms) {
    tokenValidator.validateAndConsumeTokenType(Token::Tag::Pattern);
    std::unique_ptr<Token> next = tokenValidator.validateAndConsumeSynonymToken();
    isValidPatternSynonym(next->getLexeme(), synonyms);

    tokenValidator.validateAndConsumeTokenType(Token::Tag::LParen);

    std::unique_ptr<Token> leftArg = tokenValidator.validateAndConsumePatternFirstArg();

    Tok* left = makeArg(std::move(leftArg), synonyms);

    tokenValidator.validateAndConsumeTokenType(Token::Tag::Comma);

    std::unique_ptr<Token> rightArg = tokenValidator.validateAndConsumePatternSecondArg();

    Tok* right = makeArg(std::move(rightArg), synonyms);

    tokenValidator.validateAndConsumeTokenType(Token::Tag::RParen);

    Pattern *p = new Pattern(left, right);
    return p;
}

Tok* PatternClauseParser::makeArg(std::unique_ptr<Token> token, std::vector<Synonym> synonyms) {
    if (token->getTag() == Token::Tag::Name) {
        IdentStr* i = new IdentStr(token->getLexeme());
        return i;
    } else if (token->getTag() == Token::Tag::Underscore) {
        Wildcard* w = new Wildcard();
        return w;
    } else if (token->getTag() == Token::Tag::String) {
        ExpressionStr* e = new ExpressionStr(token->getLexeme());
        return e;
    } else {
        throw SyntaxException();
    }
}

bool PatternClauseParser::isValidPatternSynonym(std::string next, std::vector<Synonym> synonyms) {
    for (auto synonym : synonyms) {
        if (next == synonym.str() && synonym.de == Synonym::DesignEntity::ASSIGN) {
            return true;
        }
    }
    throw SyntaxException();
}
