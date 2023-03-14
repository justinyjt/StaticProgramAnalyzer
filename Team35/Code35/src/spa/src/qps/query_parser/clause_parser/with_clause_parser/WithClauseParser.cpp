#include <utility>

#include "WithClauseParser.h"
#include "qps/pql/StatementNumber.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "qps/pql/Ident.h"
#include "qps/query_parser/SemanticValidator.h"
#include "qps/query_exceptions/SemanticException.h"
#include "qps/clause/TwoArgClause/WithEntClause.h"
#include "qps/clause/TwoArgClause/WithNumClause.h"

WithClauseParser::WithClauseParser(PQLTokenScanner& pqlTokenScanner,
                                           std::unordered_map<std::string, Synonym::DesignEntity>& synonyms) :
        pqlTokenScanner(pqlTokenScanner), synonyms(synonyms) {}

std::vector<std::unique_ptr<Clause>> WithClauseParser::parse() {
    std::vector<std::unique_ptr<Clause>> clauses;
    pqlTokenScanner.match(Token::Tag::With);

    clauses.push_back(parseWith());

    while (pqlTokenScanner.peek(Token::Tag::And)) {
        pqlTokenScanner.matchAndValidate(Token::Tag::And);
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
    return std::move(createClause(std::move(arg1), std::move(arg2)));
}

std::unique_ptr<PQLToken> WithClauseParser::parseRef() {
    std::string cur = pqlTokenScanner.peekLexeme();
    if (pqlTokenScanner.peek(Token::Tag::String)) {
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
        if (pqlTokenScanner.isName()) {
            de = SemanticValidator::getDesignEntity(synonyms, synonym);
            pqlTokenScanner.next();
        } else {
            throw SyntaxException();
        }

        pqlTokenScanner.match(Token::Tag::Dot);

        // parse attrName
        std::string attrName = pqlTokenScanner.peekLexeme();
        if (attrName == "procName") {
            if (de == Synonym::DesignEntity::PROCEDURE || de == Synonym::DesignEntity::CALL) {
                entArgCount++;
                pqlTokenScanner.next();
                std::unique_ptr<Synonym> s = std::make_unique<Synonym>(de, synonym);
                return std::move(s);
            }
            throw SemanticException();
        } else if (attrName == "varName") {
            if (de == Synonym::DesignEntity::VARIABLE || de == Synonym::DesignEntity::READ || de == Synonym::DesignEntity::PRINT) {
                entArgCount++;
                pqlTokenScanner.next();
                std::unique_ptr<Synonym> s = std::make_unique<Synonym>(de, synonym);
                return std::move(s);
            }
            throw SemanticException();
        } else if (attrName == "value") {
            if (de == Synonym::DesignEntity::CONSTANT) {
                numArgCount++;
                pqlTokenScanner.next();
                std::unique_ptr<Synonym> s = std::make_unique<Synonym>(de, synonym);
                return std::move(s);
            }
            throw SemanticException();
        } else if (attrName == "stmt") {
            pqlTokenScanner.next();
            if (pqlTokenScanner.peek(Token::Tag::Hex)) {
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
            throw SyntaxException();
        } else {
            throw SyntaxException();
        }
    }
}

std::unique_ptr<Clause> WithClauseParser::createClause(std::unique_ptr<PQLToken> token1,
                                                           std::unique_ptr<PQLToken> token2) {
    if (entArgCount == 1 && numArgCount == 1) {
        throw SemanticException();
    } else if (entArgCount == 2) {
        entArgCount = 0;
        numArgCount = 0;
        // create WithEnt
        std::unique_ptr<WithEntClause> w = std::make_unique<WithEntClause>(std::move(token1), std::move(token2));
        return std::move(w);
    } else if (numArgCount == 2) {
        entArgCount = 0;
        numArgCount = 0;
        // create WithNum
        std::unique_ptr<WithNumClause> w = std::make_unique<WithNumClause>(std::move(token1), std::move(token2));
        return std::move(w);
    } else {}
}