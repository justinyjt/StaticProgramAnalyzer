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

std::unique_ptr<Clause> PatternClauseParser::parse(TokenValidator& tokenValidator, std::vector<Synonym>& synonyms) {
    tokenValidator.validateAndConsumeTokenType(Token::Tag::Pattern);
    std::unique_ptr<Token> pattern = tokenValidator.validateAndConsumeSynonymToken();
    isValidPatternSynonym(pattern->getLexeme(), synonyms);

    tokenValidator.validateAndConsumeTokenType(Token::Tag::LParen);

    std::unique_ptr<Token> leftArg = tokenValidator.validateAndConsumePatternFirstArg();

    tokenValidator.validateAndConsumeTokenType(Token::Tag::Comma);

    std::vector<std::unique_ptr<Token>> rightArg = tokenValidator.validateAndConsumePatternSecondArg();

    tokenValidator.validateAndConsumeTokenType(Token::Tag::RParen);

    std::unique_ptr<Clause> clause = createClause(leftArg, rightArg,
                                                  synonyms, pattern->getLexeme());
    return clause;
}

std::unique_ptr<PQLToken> PatternClauseParser::createLeftArg(std::unique_ptr<Token>& token,
                                                             const std::vector<Synonym>& synonyms) {
    if (token->getTag() == Token::Tag::Name) {
        for (const auto& synonym : synonyms) {
            if (synonym.str() == token->getLexeme()) {
                std::unique_ptr<Synonym> s = std::make_unique<Synonym>(synonym.de, token->getLexeme());
                return std::move(s);
            }
        }
        throw SemanticException();
    } else if (token->getTag() == Token::Tag::Underscore) {
        std::unique_ptr<Wildcard> w = std::make_unique<Wildcard>();
        return std::move(w);
    } else if (token->getTag() == Token::Tag::String) {
        std::unique_ptr<Ident> ident = std::make_unique<Ident>(token->getLexeme());
        return std::move(ident);
    } else {
        throw SyntaxException();
    }
}

std::unique_ptr<PQLToken> PatternClauseParser::createRightArg(std::vector<std::unique_ptr<Token>>& tokenList) {
    if (tokenList.size() > 1) {
        std::unique_ptr<Expression> e = std::make_unique<Expression>(tokenList.at(1)->getLexeme(), true);
        return std::move(e);
    } else if (tokenList.at(0)->getTag() == Token::Tag::Underscore) {
        std::unique_ptr<Wildcard> w = std::make_unique<Wildcard>();
        return std::move(w);
    } else {
        std::unique_ptr<Expression> e = std::make_unique<Expression>(tokenList.at(0)->getLexeme(), false);
        return std::move(e);
    }
}

std::unique_ptr<Clause> PatternClauseParser::createClause(std::unique_ptr<Token>& token1,
                                                          std::vector<std::unique_ptr<Token>>& token2,
                                                          std::vector<Synonym>& synonyms,
                                                          const std::string& patternStr) {
    // entRef - variable synonyms, _ , string
    // _ , exact match, partial match

    std::unique_ptr<PQLToken> first = createLeftArg(token1, synonyms);
    std::unique_ptr<PQLToken> second = createRightArg(token2);
    if (isEntRef(*first)) {
        std::unique_ptr<Clause> a = std::make_unique<Pattern>(std::move(first), std::move(second), patternStr);
        return std::move(a);
    }
    throw SyntaxException();
}

bool PatternClauseParser::isValidPatternSynonym(const std::string& next, std::vector<Synonym>& synonyms) {
    for (const auto& synonym : synonyms) {
        if (next == synonym.str() && synonym.de == Synonym::DesignEntity::ASSIGN) {
            return true;
        }
    }
    throw SemanticException();
}

bool PatternClauseParser::isEntRef(PQLToken& tok) {
    const auto* synonym = dynamic_cast<const Synonym*>(&tok);
    return tok.tag == PQLToken::Tag::IDENT ||
           tok.tag == PQLToken::Tag::WILDCARD ||
           synonym != nullptr && (synonym->de == Synonym::DesignEntity::VARIABLE ||
                               synonym->de == Synonym::DesignEntity::CONSTANT);
}
