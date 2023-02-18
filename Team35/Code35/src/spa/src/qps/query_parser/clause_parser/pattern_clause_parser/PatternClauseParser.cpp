#include <string>
#include "PatternClauseParser.h"
#include "qps/clause/Pattern.h"
#include "qps/pql/Synonym.h"
#include "qps/pql/Wildcard.h"
#include "qps/pql/Expression.h"
#include "qps/pql/StatementNumber.h"
#include "qps/pql/Ident.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "qps/query_exceptions/SemanticException.h"

Clause *PatternClauseParser::parse(TokenValidator &tokenValidator, std::vector<Synonym> synonyms) {
    tokenValidator.validateAndConsumeTokenType(Token::Tag::Pattern);
    std::unique_ptr<Token> pattern = tokenValidator.validateAndConsumeSynonymToken();
    isValidPatternSynonym(pattern->getLexeme(), synonyms);

    tokenValidator.validateAndConsumeTokenType(Token::Tag::LParen);

    std::unique_ptr<Token> leftArg = tokenValidator.validateAndConsumePatternFirstArg();

    tokenValidator.validateAndConsumeTokenType(Token::Tag::Comma);

    std::vector<std::unique_ptr<Token>> rightArg = tokenValidator.validateAndConsumePatternSecondArg();

    tokenValidator.validateAndConsumeTokenType(Token::Tag::RParen);

    Clause* clause = createClause(std::move(leftArg), std::move(rightArg), synonyms, pattern->getLexeme());
    return clause;
}

PQLToken* PatternClauseParser::createLeftArg(std::unique_ptr<Token> &token, std::vector<Synonym> synonyms) {
    if (token->getTag() == Token::Tag::Name) {
        for (auto synonym : synonyms) {
            if (synonym.str() == token->getLexeme()) {
                Synonym* s = new Synonym(synonym.de, token->getLexeme());
                return s;
            }
        }
        throw SemanticException();
    } else if (token->getTag() == Token::Tag::Underscore) {
        auto* w = new Wildcard();
        return w;
    } else if (token->getTag() == Token::Tag::String) {
        Ident* ident = new Ident(token->getLexeme());
        return ident;
    } else {
        throw SyntaxException();
    }
}

PQLToken* PatternClauseParser::createRightArg(std::vector<std::unique_ptr<Token>> &tokenList) {
    if (tokenList.size() > 1) {
        auto* e = new Expression(tokenList.at(1)->getLexeme(), true);
        return e;
    } else if (tokenList.at(0)->getTag() == Token::Tag::Underscore) {
        auto* w = new Wildcard();
        return w;
    } else {
        auto* e = new Expression(tokenList.at(0)->getLexeme(), false);
        return e;
    }
}

Clause* PatternClauseParser::createClause(std::unique_ptr<Token> token1, std::vector<std::unique_ptr<Token>> token2,
                                          std::vector<Synonym> synonyms, std::string patternStr) {
    // entRef - variable synonyms, _ , string
    // _ , exact match, partial match

    PQLToken* first = createLeftArg(token1, synonyms);
    PQLToken* second = createRightArg(token2);
    if (isEntRef(*first)) {
        Pattern *a = new Pattern(first, second, patternStr);
        return a;
    }
    throw SyntaxException();
}

bool PatternClauseParser::isValidPatternSynonym(const std::string& next, std::vector<Synonym> synonyms) {
    for (auto synonym : synonyms) {
        if (next == synonym.str() && synonym.de == Synonym::DesignEntity::ASSIGN) {
            return true;
        }
    }
    throw SemanticException();
}

bool PatternClauseParser::isEntRef(PQLToken &tok) {
    const Synonym* synonym = dynamic_cast<const Synonym*>(&tok);
    return tok.tag == PQLToken::Tag::IDENT ||
           tok.tag == PQLToken::Tag::WILDCARD ||
           synonym != NULL && (synonym->de == Synonym::DesignEntity::VARIABLE ||
                               synonym->de == Synonym::DesignEntity::CONSTANT);
}
