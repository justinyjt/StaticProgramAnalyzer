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

Clause *SuchThatClauseParser::parse(TokenValidator &tokenValidator, std::vector<Synonym> synonyms) {
    tokenValidator.validateAndConsumeTokenType(Token::Tag::Such);
    tokenValidator.validateAndConsumeTokenType(Token::Tag::That);

    std::string relationship = tokenValidator.validateAndConsumeRelationship();

    tokenValidator.validateAndConsumeTokenType(Token::Tag::LParen);

    std::unique_ptr<Token> leftArg = tokenValidator.validateAndConsumeRelationshipArg();

    tokenValidator.validateAndConsumeTokenType(Token::Tag::Comma);

    std::unique_ptr<Token> rightArg = tokenValidator.validateAndConsumeRelationshipArg();

    tokenValidator.validateAndConsumeTokenType(Token::Tag::RParen);

    Clause* clause = createClause(std::move(leftArg),
                                  std::move(rightArg), relationship, std::move(synonyms));

    return clause;
}

PQLToken* SuchThatClauseParser::createArg(std::unique_ptr<Token>& token, const std::vector<Synonym>& synonyms) {
    if (token->getTag() == Token::Tag::Integer) {
        StatementNumber* t = new StatementNumber(stoi(token->getLexeme()));
        return t;
    } else if (token->getTag() == Token::Tag::String) {
        Ident* i = new Ident(token->getLexeme());
        return i;
    } else if (token->getTag() == Token::Tag::Underscore) {
        Wildcard* w = new Wildcard();
        return w;
    } else if (token->getTag() == Token::Tag::Name) {
        for (auto synonym : synonyms) {
            if (synonym.str() == token->getLexeme()) {
                Synonym* s = new Synonym(synonym.de, token->getLexeme());
                return s;
            }
        }
        throw SemanticException();
    } else {
        throw SyntaxException();
    }
}

Clause* SuchThatClauseParser::createClause(std::unique_ptr<Token> token1, std::unique_ptr<Token> token2,
                                           std::string relationship, std::vector<Synonym> synonyms) {
    PQLToken* first = createArg(token1, synonyms);
    PQLToken* second = createArg(token2, synonyms);

    if (relationship == "Modifies") {
        if (isStmtRef(*first) && isEntRef(*second)) {
            ModifiesS *m = new ModifiesS(first, second);
            return m;
        }
        throw SyntaxException();
    } else if (relationship == "Uses") {
        if (isStmtRef(*first) && isEntRef(*second)) {
            UsesS *u = new UsesS(first, second);
            return u;
        }
        throw SyntaxException();
    } else if (relationship == "Follows") {
        if (isStmtRef(*first) && isStmtRef(*second)) {
            Follows *f = new Follows(first, second, false);
            return f;
        }
        throw SyntaxException();
    } else if (relationship == "Follows*") {
        if (isStmtRef(*first) && isStmtRef(*second)) {
            Follows *f = new Follows(first, second, true);
            return f;
        }
        throw SyntaxException();
    } else if (relationship == "Parent") {
        if (isStmtRef(*first) && isStmtRef(*second)) {
            Parent *p = new Parent(first, second, false);
            return p;
        }
        throw SyntaxException();
    } else if (relationship == "Parent*") {
        if (isStmtRef(*first) && isStmtRef(*second)) {
            Parent *p = new Parent(first, second, true);
            return p;
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
