#include <string>
#include "PatternClauseParser.h"
#include "qps/clause/pattern/Pattern.h"
#include "qps/pql/Synonym.h"
#include "qps/pql/Wildcard.h"
#include "qps/pql/IdentStr.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "qps/query_exceptions/SemanticException.h"
#include "qps/pql/IdentStrWithWildcard.h"

Clause *PatternClauseParser::parse(TokenValidator &tokenValidator, std::vector<Synonym> synonyms) {
    tokenValidator.validateAndConsumeTokenType(Token::Tag::Pattern);
    std::unique_ptr<Token> next = tokenValidator.validateAndConsumeSynonymToken();
    isValidPatternSynonym(next->getLexeme(), synonyms);

    tokenValidator.validateAndConsumeTokenType(Token::Tag::LParen);

    std::unique_ptr<Token> leftArg = tokenValidator.validateAndConsumePatternFirstArg();

    tokenValidator.validateAndConsumeTokenType(Token::Tag::Comma);

    std::vector<std::unique_ptr<Token>> rightArg = tokenValidator.validateAndConsumePatternSecondArg();

    tokenValidator.validateAndConsumeTokenType(Token::Tag::RParen);

    Clause* clause = createClause(std::move(leftArg), std::move(rightArg), synonyms);
    return clause;
}

Tok* PatternClauseParser::createLeftArg(std::unique_ptr<Token> &token, std::vector<Synonym> synonyms) {
    if (token->getTag() == Token::Tag::Name) {
        for (auto synonym : synonyms) {
            if (synonym.str() == token->getLexeme()) {
                Synonym* s = new Synonym(synonym.de, token->getLexeme());
                return s;
            }
        }
        throw SyntaxException();
    } else if (token->getTag() == Token::Tag::Underscore) {
        auto* w = new Wildcard();
        return w;
    } else if (token->getTag() == Token::Tag::String) {
        auto* i = new IdentStr(token->getLexeme());
        return i;
    } else {
        throw SyntaxException();
    }
}

Tok* PatternClauseParser::createRightArg(std::vector<std::unique_ptr<Token>> &tokenList) {
    if (tokenList.size() > 1) {
        auto* i = new IdentStrWithWildcard(tokenList.at(1)->getLexeme());
        return i;
    } else {
        auto* w = new Wildcard();
        return w;
    }
}

Clause* PatternClauseParser::createClause(std::unique_ptr<Token> token1, std::vector<std::unique_ptr<Token>> token2,
                                          std::vector<Synonym> synonyms) {
    // entRef - variable synonyms, _ , string
    // _ , exact match, partial match

    Tok* first = createLeftArg(token1, synonyms);
    Tok* second = createRightArg(token2);
    if (isEntRef(*first)) {
        Pattern *a = new Pattern(first, second);
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
    throw SyntaxException();
}

bool PatternClauseParser::isEntRef(Tok &tok) {
    const Synonym* synonym = dynamic_cast<const Synonym*>(&tok);
    return tok.tag == Tok::Tag::IDENT ||
           tok.tag == Tok::Tag::WILDCARD ||
           synonym != NULL && (synonym->de == Synonym::DesignEntity::VARIABLE ||
                               synonym->de == Synonym::DesignEntity::CONSTANT);
}
