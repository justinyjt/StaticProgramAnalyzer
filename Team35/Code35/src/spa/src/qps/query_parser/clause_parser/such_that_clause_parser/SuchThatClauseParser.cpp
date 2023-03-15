#include <utility>

#include "SuchThatClauseParser.h"
#include "qps/pql/StatementNumber.h"
#include "qps/pql/Wildcard.h"
#include "qps/clause/TwoArgClause/StmtEntClause.h"
#include "qps/pql/Ident.h"
#include "qps/query_parser/SemanticValidator.h"
#include "qps/clause/TwoArgClause/TwoArgClauseFactory.h"
#include "qps/query_parser/helper.h"

SuchThatClauseParser::SuchThatClauseParser(PQLTokenScanner& pqlTokenScanner,
                                           std::unordered_map<std::string, Synonym::DesignEntity>& synonyms) :
    pqlTokenScanner(pqlTokenScanner), synonyms(synonyms) {}

std::vector<std::unique_ptr<Clause>> SuchThatClauseParser::parse() {
    std::vector<std::unique_ptr<Clause>> clauses;
    pqlTokenScanner.match(Token::Tag::Such);
    pqlTokenScanner.match(Token::Tag::That);

    clauses.push_back(parseRelationship());

    while (pqlTokenScanner.peek(Token::Tag::And)) {
        pqlTokenScanner.match(Token::Tag::And);
        clauses.push_back(parseRelationship());
    }

    return clauses;
}

std::unique_ptr<Clause> SuchThatClauseParser::parseRelationship() {
    std::string relationship;
    if (pqlTokenScanner.peek(Token::Tag::Modifies) || pqlTokenScanner.peek(Token::Tag::Uses)) {
        relationship += pqlTokenScanner.peekLexeme();
        pqlTokenScanner.next();
        return std::move(parseUsesModifies(relationship));
    } else if (pqlTokenScanner.peek(Token::Tag::Parent) || pqlTokenScanner.peek(Token::Tag::Follows)) {
        relationship += pqlTokenScanner.peekLexeme();
        pqlTokenScanner.next();
        if (pqlTokenScanner.peek(Token::Tag::Star)) {
            relationship += pqlTokenScanner.peekLexeme();
            pqlTokenScanner.next();
        }
        return std::move(parseStmtStmt(relationship));
    } else if (pqlTokenScanner.peek(Token::Tag::Calls)) {
        relationship += pqlTokenScanner.peekLexeme();
        pqlTokenScanner.next();
        if (pqlTokenScanner.peek(Token::Tag::Star)) {
            relationship += pqlTokenScanner.peekLexeme();
            pqlTokenScanner.next();
        }
        return std::move(parseEntEnt(relationship));
    }
}

std::unique_ptr<Clause> SuchThatClauseParser::parseUsesModifies(std::string& relationship) {
    std::unique_ptr<PQLToken> arg1;
    std::unique_ptr<PQLToken> arg2;
    pqlTokenScanner.match(Token::Tag::LParen);
    if (pqlTokenScanner.isName()) {
        std::string synonym = pqlTokenScanner.peekLexeme();
        Synonym::DesignEntity de = SemanticValidator::getDesignEntity(synonyms, synonym);
        arg1 = std::make_unique<Synonym>(de, synonym);
        if (de == Synonym::DesignEntity::STMT || de == Synonym::DesignEntity::READ ||
            de == Synonym::DesignEntity::PRINT || de == Synonym::DesignEntity::ASSIGN ||
            de == Synonym::DesignEntity::CALL || de == Synonym::DesignEntity::WHILE ||
            de == Synonym::DesignEntity::IF) {
            relationship += USES_MODIFIES_S_APPEND;
        } else {
            relationship += USES_MODIFIES_P_APPEND;
        }
        pqlTokenScanner.next();
    } else if (pqlTokenScanner.peek(Token::Tag::Underscore)) {
        arg1 = std::make_unique<Wildcard>();
        relationship += USES_MODIFIES_S_APPEND;
        pqlTokenScanner.next();
    } else if (pqlTokenScanner.peek(Token::Tag::String)) {
        arg1 = std::make_unique<Ident>(pqlTokenScanner.peekLexeme());
        relationship += USES_MODIFIES_P_APPEND;
        pqlTokenScanner.next();
    } else if (pqlTokenScanner.peek(Token::Tag::Integer)) {
        arg1 = std::make_unique<StatementNumber>(
                stoi(pqlTokenScanner.peekLexeme()));
        relationship += USES_MODIFIES_S_APPEND;
        pqlTokenScanner.next();
    }

    pqlTokenScanner.match(Token::Tag::Comma);
    arg2 = parseEntRef();
    pqlTokenScanner.match(Token::Tag::RParen);

    return std::move(TwoArgClauseFactory::createClause(std::move(arg1), std::move(arg2), relationship));
}

std::unique_ptr<Clause> SuchThatClauseParser::parseStmtStmt(std::string& relationship) {
    std::unique_ptr<PQLToken> arg1;
    std::unique_ptr<PQLToken> arg2;

    pqlTokenScanner.match(Token::Tag::LParen);
    arg1 = parseStmtRef();

    pqlTokenScanner.match(Token::Tag::Comma);
    arg2 = parseStmtRef();

    pqlTokenScanner.match(Token::Tag::RParen);
    return std::move(TwoArgClauseFactory::createClause(std::move(arg1), std::move(arg2), relationship));
}

std::unique_ptr<Clause> SuchThatClauseParser::parseEntEnt(std::string& relationship) {
    std::unique_ptr<PQLToken> arg1;
    std::unique_ptr<PQLToken> arg2;

    pqlTokenScanner.match(Token::Tag::LParen);
    arg1 = parseEntRef();

    pqlTokenScanner.match(Token::Tag::Comma);
    arg2 = parseEntRef();

    pqlTokenScanner.match(Token::Tag::RParen);
    return std::move(TwoArgClauseFactory::createClause(std::move(arg1), std::move(arg2), relationship));
}

std::unique_ptr<PQLToken> SuchThatClauseParser::parseEntRef() {
    if (pqlTokenScanner.isName()) {
        std::string synonym = pqlTokenScanner.peekLexeme();
        Synonym::DesignEntity de = SemanticValidator::getDesignEntity(synonyms, synonym);
        pqlTokenScanner.next();
        std::unique_ptr<Synonym> s = std::make_unique<Synonym>(de, synonym);
        return std::move(s);
    } else if (pqlTokenScanner.peek(Token::Tag::Underscore)) {
        std::unique_ptr<Wildcard> w = std::make_unique<Wildcard>();
        pqlTokenScanner.next();
        return w;
    } else if (pqlTokenScanner.peek(Token::Tag::String)) {
        std::unique_ptr<Ident> i = std::make_unique<Ident>(pqlTokenScanner.peekLexeme());
        pqlTokenScanner.next();
        return i;
    }
}

std::unique_ptr<PQLToken> SuchThatClauseParser::parseStmtRef() {
    if (pqlTokenScanner.isName()) {
        std::string synonym = pqlTokenScanner.peekLexeme();
        Synonym::DesignEntity de = SemanticValidator::getDesignEntity(synonyms, synonym);
        pqlTokenScanner.next();
        std::unique_ptr<Synonym> s = std::make_unique<Synonym>(de, synonym);
        return std::move(s);
    } else if (pqlTokenScanner.peek(Token::Tag::Underscore)) {
        std::unique_ptr<Wildcard> w = std::make_unique<Wildcard>();
        pqlTokenScanner.next();
        return w;
    } else if (pqlTokenScanner.peek(Token::Tag::Integer)) {
        std::unique_ptr<StatementNumber> s = std::make_unique<StatementNumber>(stoi(pqlTokenScanner.peekLexeme()));
        pqlTokenScanner.next();
        return s;
    }
}
