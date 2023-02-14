#include "SuchThatClauseParser.h"

#include <utility>

#include "qps/clause/relationship/Modify.h"
#include "qps/pql/StatementNumber.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "qps/pql/Wildcard.h"
#include "qps/pql/IdentStr.h"
#include "qps/clause/relationship/Uses.h"
#include "qps/clause/relationship/Follows.h"
#include "qps/clause/relationship/Parent.h"
#include "qps/query_exceptions/SemanticException.h"

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
                                  std::move(rightArg), relationship, synonyms);

    return clause;
}

Tok* SuchThatClauseParser::createArg(std::unique_ptr<Token>& token, std::vector<Synonym> synonyms) {
    if (token->getTag() == Token::Tag::Integer) {
        StatementNumber* t = new StatementNumber(stoi(token->getLexeme()));
        return t;
    } else if (token->getTag() == Token::Tag::String) {
        IdentStr* i = new IdentStr(token->getLexeme());
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
    // Follows, Modifies, Uses, Parent
    // stmtRef - stmt, read, print, assign, call, while, if synonyms, _ , integer
    // entRef - variable, constant, procedure synonyms, _ , "IDENT"

    Tok* first = createArg(token1, synonyms);
    Tok* second = createArg(token2, synonyms);
    if (relationship == "Modifies") {
        if (isStmtRef(*first) && isEntRef(*second)) {
            ModifiesS *m = new ModifiesS(first, second);
            return m;
        } else {
            ModifiesP *m = new ModifiesP(first, second);
            return m;
        }
    } else if (relationship == "Uses") {
        if (isStmtRef(*first) && isEntRef(*second)) {
            UsesS *u = new UsesS(first, second);
            return u;
        } else {
            UsesP *u = new UsesP(first, second);
            return u;
        }
    } else if (relationship == "Follows") {
        if (isStmtRef(*first) && isStmtRef(*second)) {
            Follows *f = new Follows(first, second);
            return f;
        }
        throw SyntaxException();
    } else if (relationship == "Follows*") {
        if (isStmtRef(*first) && isStmtRef(*second)) {
            FollowsT *f = new FollowsT(first, second);
            return f;
        }
        throw SyntaxException();
    } else if (relationship == "Parent") {
        if (isStmtRef(*first) && isStmtRef(*second)) {
            Parent *p = new Parent(first, second);
            return p;
        }
        throw SyntaxException();
    } else if (relationship == "Parent*") {
        if (isStmtRef(*first) && isStmtRef(*second)) {
            ParentT *p = new ParentT(first, second);
            return p;
        }
        throw SyntaxException();
    }
}

bool SuchThatClauseParser::isStmtRef(Tok &tok) {
    const Synonym* synonym = dynamic_cast<const Synonym*>(&tok);
    return tok.tag == Tok::Tag::STMT_NUM ||
            tok.tag == Tok::Tag::WILDCARD ||
            synonym != NULL && (synonym->de == Synonym::DesignEntity::STMT ||
                    synonym->de == Synonym::DesignEntity::READ ||
                    synonym->de == Synonym::DesignEntity::PRINT ||
                    synonym->de == Synonym::DesignEntity::ASSIGN ||
                    synonym->de == Synonym::DesignEntity::CALL ||
                    synonym->de == Synonym::DesignEntity::WHILE ||
                    synonym->de == Synonym::DesignEntity::IF);
}

bool SuchThatClauseParser::isEntRef(Tok &tok) {
    const Synonym* synonym = dynamic_cast<const Synonym*>(&tok);
    return tok.tag == Tok::Tag::IDENT ||
           tok.tag == Tok::Tag::WILDCARD ||
           synonym != NULL && (synonym->de == Synonym::DesignEntity::VARIABLE ||
                               synonym->de == Synonym::DesignEntity::CONSTANT);
}
