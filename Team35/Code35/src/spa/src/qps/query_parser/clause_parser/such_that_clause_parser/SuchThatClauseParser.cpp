#include "SuchThatClauseParser.h"

#include <utility>

#include "qps/clause/Modifies.h"
#include "qps/pql/StatementNumber.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "qps/pql/Wildcard.h"
#include "qps/clause/Uses.h"
#include "qps/clause/Follows.h"
#include "qps/clause/Parent.h"
#include "qps/query_exceptions/SemanticException.h"
#include "qps/pql/Ident.h"

std::unique_ptr<Clause> SuchThatClauseParser::parse(TokenValidator &tokenValidator, std::vector<Synonym> synonyms) {
    tokenValidator.validateAndConsumeTokenType(Token::Tag::Such);
    tokenValidator.validateAndConsumeTokenType(Token::Tag::That);

    std::string relationship = tokenValidator.validateAndConsumeRelationship();

    tokenValidator.validateAndConsumeTokenType(Token::Tag::LParen);

    std::unique_ptr<Token> leftArg = tokenValidator.validateAndConsumeRelationshipArg();

    tokenValidator.validateAndConsumeTokenType(Token::Tag::Comma);

    std::unique_ptr<Token> rightArg = tokenValidator.validateAndConsumeRelationshipArg();

    tokenValidator.validateAndConsumeTokenType(Token::Tag::RParen);

    std::unique_ptr<Clause> clause = createClause(std::move(leftArg),
                                  std::move(rightArg), relationship, std::move(synonyms));

    return clause;
}

std::shared_ptr<PQLToken> SuchThatClauseParser::createArg(std::unique_ptr<Token>& token,
                                                          const std::vector<Synonym>& synonyms) {
    if (token->getTag() == Token::Tag::Integer) {
        std::shared_ptr<StatementNumber> t = std::make_shared<StatementNumber>(stoi(token->getLexeme()));
        return std::move(t);
    } else if (token->getTag() == Token::Tag::String) {
        std::shared_ptr<Ident> i = std::make_shared<Ident>(token->getLexeme());
        return std::move(i);
    } else if (token->getTag() == Token::Tag::Underscore) {
        std::shared_ptr<Wildcard> w = std::make_shared<Wildcard>();
        return std::move(w);
    } else if (token->getTag() == Token::Tag::Name) {
        for (auto synonym : synonyms) {
            if (synonym.str() == token->getLexeme()) {
                std::shared_ptr<Synonym> s = std::make_shared<Synonym>(synonym.de, token->getLexeme());
                return std::move(s);
            }
        }
        throw SemanticException();
    } else {
        throw SyntaxException();
    }
}

std::unique_ptr<Clause> SuchThatClauseParser::createClause(std::unique_ptr<Token> token1, std::unique_ptr<Token> token2,
                                           std::string relationship, std::vector<Synonym> synonyms) {
    std::shared_ptr<PQLToken> first = createArg(token1, synonyms);
    std::shared_ptr<PQLToken> second = createArg(token2, synonyms);

    if (relationship == "Modifies") {
        if (isStmtRef(*first) && isEntRef(*second)) {
            std::unique_ptr<ModifiesS> m = std::make_unique<ModifiesS>(std::move(first), std::move(second));
            return std::move(m);
        }
        throw SyntaxException();
    } else if (relationship == "Uses") {
        if (isStmtRef(*first) && isEntRef(*second)) {
            std::unique_ptr<UsesS> u = std::make_unique<UsesS>(std::move(first), std::move(second));
            return std::move(u);
        }
        throw SyntaxException();
    } else if (relationship == "Follows") {
        if (isStmtRef(*first) && isStmtRef(*second)) {
            std::unique_ptr<Follows> f = std::make_unique<Follows>(first, second, false);
            return std::move(f);
        }
        throw SyntaxException();
    } else if (relationship == "Follows*") {
        if (isStmtRef(*first) && isStmtRef(*second)) {
            std::unique_ptr<Follows> f = std::make_unique<Follows>(first, second, true);
            return std::move(f);
        }
        throw SyntaxException();
    } else if (relationship == "Parent") {
        if (isStmtRef(*first) && isStmtRef(*second)) {
            std::unique_ptr<Parent> p = std::make_unique<Parent>(first, second, false);
            return std::move(p);
        }
        throw SyntaxException();
    } else if (relationship == "Parent*") {
        if (isStmtRef(*first) && isStmtRef(*second)) {
            std::unique_ptr<Parent> p = std::make_unique<Parent>(first, second, true);
            return std::move(p);
        }
        throw SyntaxException();
    }
    throw SyntaxException();
}

bool SuchThatClauseParser::isStmtRef(PQLToken &tok) {
    const Synonym* synonym = dynamic_cast<const Synonym*>(&tok);
    return tok.tag == PQLToken::Tag::STMT_NUM ||
            tok.tag == PQLToken::Tag::WILDCARD ||
            synonym != NULL && (synonym->de == Synonym::DesignEntity::STMT ||
                    synonym->de == Synonym::DesignEntity::READ ||
                    synonym->de == Synonym::DesignEntity::PRINT ||
                    synonym->de == Synonym::DesignEntity::ASSIGN ||
                    synonym->de == Synonym::DesignEntity::CALL ||
                    synonym->de == Synonym::DesignEntity::WHILE ||
                    synonym->de == Synonym::DesignEntity::IF);
}

bool SuchThatClauseParser::isEntRef(PQLToken &tok) {
    const Synonym* synonym = dynamic_cast<const Synonym*>(&tok);
    return tok.tag == PQLToken::Tag::IDENT ||
           tok.tag == PQLToken::Tag::WILDCARD ||
           synonym != NULL && (synonym->de == Synonym::DesignEntity::VARIABLE ||
                               synonym->de == Synonym::DesignEntity::CONSTANT ||
                                synonym->de == Synonym::DesignEntity::PROCEDURE);
}
