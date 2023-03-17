#include <utility>

#include "WithClauseParser.h"
#include "qps/pql/StatementNumber.h"
#include "qps/pql/Ident.h"
#include "qps/query_parser/SemanticValidator.h"
#include "qps/query_exceptions/SemanticException.h"
#include "qps/clause/TwoArgClause/WithEntClause.h"
#include "qps/clause/TwoArgClause/WithNumClause.h"
#include "qps/clause/TwoArgClause/TwoArgClauseFactory.h"
#include "qps/query_parser/helper.h"

WithClauseParser::WithClauseParser(PQLTokenScanner& pqlTokenScanner,
                                           std::unordered_map<std::string, Synonym::DesignEntity>& synonyms) :
        pqlTokenScanner(pqlTokenScanner), synonyms(synonyms) {}

std::vector<std::unique_ptr<Clause>> WithClauseParser::parse() {
    std::vector<std::unique_ptr<Clause>> clauses;
    pqlTokenScanner.match(Token::Tag::With);

    clauses.push_back(parseWith());

    while (pqlTokenScanner.peek(Token::Tag::And)) {
        pqlTokenScanner.match(Token::Tag::And);
        clauses.push_back(parseWith());
    }

    return clauses;
}

std::unique_ptr<Clause> WithClauseParser::parseWith() {
    std::unique_ptr<PQLToken> arg1;
    std::unique_ptr<PQLToken> arg2;
    arg1 = parseRef();

    // parse equal
    pqlTokenScanner.match(Token::Tag::Equal);

    arg2 = parseRef();

    if (entArgCount == 1 && numArgCount == 1) {
        throw SemanticException();
    } else if (entArgCount == 2) {
        entArgCount = 0;
        numArgCount = 0;
        // create WithEnt
        return std::move(TwoArgClauseFactory::createClause(std::move(arg1), std::move(arg2), WITHENT_KEYWORD));
    } else if (numArgCount == 2) {
        entArgCount = 0;
        numArgCount = 0;
        // create WithNum
        return std::move(TwoArgClauseFactory::createClause(std::move(arg1), std::move(arg2), WITHNUM_KEYWORD));
    }
}

std::unique_ptr<PQLToken> WithClauseParser::parseRef() {
    std::string cur = pqlTokenScanner.peekLexeme();
    if (pqlTokenScanner.peekIdent()) {
        std::unique_ptr<Ident> i = std::make_unique<Ident>(pqlTokenScanner.peekLexeme());
        pqlTokenScanner.next();
        entArgCount++;
        return i;
    } else if (pqlTokenScanner.peek(Token::Tag::Integer)) {
        std::unique_ptr<StatementNumber> s = std::make_unique<StatementNumber>(stoi(pqlTokenScanner.peekLexeme()));
        pqlTokenScanner.next();
        numArgCount++;
        return s;
    } else {
        // parse attrRef

        // parse synonym
        Synonym::DesignEntity de;
        std::string synonym = pqlTokenScanner.peekLexeme();
        de = SemanticValidator::getDesignEntity(synonyms, synonym);
        pqlTokenScanner.next();

        pqlTokenScanner.match(Token::Tag::Dot);

        // parse attrName
        std::string attrName = pqlTokenScanner.peekLexeme();
        if (attrName == PROCNAME_KEYWORD) {
            if (de == Synonym::DesignEntity::PROCEDURE || de == Synonym::DesignEntity::CALL) {
                entArgCount++;
                pqlTokenScanner.next();
                std::unique_ptr<Synonym> s = std::make_unique<Synonym>(de, synonym);
                return std::move(s);
            }
            throw SemanticException();
        } else if (attrName == VARNAME_KEYWORD) {
            if (de == Synonym::DesignEntity::VARIABLE || de == Synonym::DesignEntity::READ ||
                de == Synonym::DesignEntity::PRINT) {
                entArgCount++;
                pqlTokenScanner.next();
                std::unique_ptr<Synonym> s = std::make_unique<Synonym>(de, synonym);
                return std::move(s);
            }
            throw SemanticException();
        } else if (attrName == VALUE_KEYWORD) {
            if (de == Synonym::DesignEntity::CONSTANT) {
                numArgCount++;
                pqlTokenScanner.next();
                std::unique_ptr<Synonym> s = std::make_unique<Synonym>(de, synonym);
                return std::move(s);
            }
            throw SemanticException();
        } else if (attrName == STMT_KEYWORD) {
            pqlTokenScanner.next();
            if (de == Synonym::DesignEntity::STMT || de == Synonym::DesignEntity::READ ||
                de == Synonym::DesignEntity::PRINT || de == Synonym::DesignEntity::CALL ||
                de == Synonym::DesignEntity::WHILE || de == Synonym::DesignEntity::IF ||
                de == Synonym::DesignEntity::ASSIGN) {
                numArgCount++;
                pqlTokenScanner.next();
                std::unique_ptr<Synonym> s = std::make_unique<Synonym>(de, synonym);
                return std::move(s);
            }
            throw SemanticException();
        }
    }
}