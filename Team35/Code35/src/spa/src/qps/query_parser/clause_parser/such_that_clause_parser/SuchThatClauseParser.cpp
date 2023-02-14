#include "SuchThatClauseParser.h"

#include <utility>

#include "qps/clause/relationship/Modifies.h"
#include "qps/pql/StatementNumber.h"
#include "qps/query_exceptions/SyntaxException.h"

Clause *SuchThatClauseParser::parse(TokenValidator &tokenValidator, std::vector<Synonym> synonyms) {
    tokenValidator.validateAndConsumeTokenType(Token::Tag::Such);
    tokenValidator.validateAndConsumeTokenType(Token::Tag::That);

    std::unique_ptr<Token> relationship = tokenValidator.validateAndConsumeRelationship();

    tokenValidator.validateAndConsumeTokenType(Token::Tag::LParen);

    std::unique_ptr<Token> leftArg = tokenValidator.validateAndConsumeRelationshipArg();

    Tok* left = makeArg(std::move(leftArg), synonyms);

    tokenValidator.validateAndConsumeTokenType(Token::Tag::Comma);

    std::unique_ptr<Token> rightArg = tokenValidator.validateAndConsumeRelationshipArg();

    Tok* right = makeArg(std::move(rightArg), synonyms);

    tokenValidator.validateAndConsumeTokenType(Token::Tag::RParen);

    ModifiesS *m = new ModifiesS(left, right);
    return m;
}

Tok* SuchThatClauseParser::makeArg(std::unique_ptr<Token> token, std::vector<Synonym> synonyms) {
    if (token->getTag() == Token::Tag::Integer) {
        StatementNumber* t = new StatementNumber(stoi(token->getLexeme()));
        return t;
    } else if (token->getTag() == Token::Tag::Name) {
        for (auto synonym : synonyms) {
            if (synonym.str() == token->getLexeme()) {
                Synonym* s = new Synonym(synonym.de, token->getLexeme());
                return s;
            }
        }
        throw SyntaxException();
    } else {
        throw SyntaxException();
    }
}
