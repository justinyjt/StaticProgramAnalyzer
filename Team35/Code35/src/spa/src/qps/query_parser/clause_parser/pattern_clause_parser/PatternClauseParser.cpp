#include <string>
#include "PatternClauseParser.h"
#include "qps/clause/TwoArgClause/AssignPattern.h"
#include "qps/pql/Synonym.h"
#include "qps/pql/Wildcard.h"
#include "qps/pql/Expression.h"
#include "qps/pql/Ident.h"
#include "qps/query_exceptions/SemanticException.h"
#include "commons/token_scanner/TokenScanner.h"
#include "qps/query_parser/SemanticValidator.h"
#include "qps/clause/one_arg_clause/WhilePattern.h"
#include "qps/clause/one_arg_clause/IfPattern.h"

PatternClauseParser::PatternClauseParser(PQLTokenScanner& pqlTokenScanner,
                                         std::unordered_map<std::string, Synonym::DesignEntity>& synonyms) :
        pqlTokenScanner(pqlTokenScanner), synonyms(synonyms) {}

std::vector<std::unique_ptr<Clause>> PatternClauseParser::parse() {
    std::vector<std::unique_ptr<Clause>> clauses;
    pqlTokenScanner.match(Token::Tag::Pattern);

    clauses.push_back(parsePattern());

    while (pqlTokenScanner.peek(Token::Tag::And)) {
        pqlTokenScanner.match(Token::Tag::And);
        clauses.push_back(parsePattern());
    }

    return clauses;
}

std::unique_ptr<Clause> PatternClauseParser::parsePattern() {
    // validate and return type
    assert(pqlTokenScanner.isName());
    Synonym::DesignEntity de;
    std::string pattern = pqlTokenScanner.peekLexeme();
    de = parsePatternSynonym();

    pqlTokenScanner.saveState();
    pqlTokenScanner.next();
    pqlTokenScanner.match(Token::Tag::LParen);
    pqlTokenScanner.next();
    pqlTokenScanner.match(Token::Tag::Comma);
    if (pqlTokenScanner.peek(Token::Tag::Underscore)) {
        pqlTokenScanner.match(Token::Tag::Underscore);
        if (pqlTokenScanner.peek(Token::Tag::String)) {  // _"expr"_ -> assign
            pqlTokenScanner.restoreState();
            return parseAssign(pattern);
        } else if (pqlTokenScanner.peek(Token::Tag::RParen)) {  // _) -> assign/while
            // depends on de
            pqlTokenScanner.restoreState();
            switch (de) {
                case Synonym::DesignEntity::ASSIGN:
                    return parseAssign(pattern);
                case Synonym::DesignEntity::WHILE:
                    return parseWhile(pattern);
                default:
                {}
            }
        } else {  // _, -> if
            pqlTokenScanner.restoreState();
            return parseIf(pattern);
        }
    } else if (pqlTokenScanner.peek(Token::Tag::String)) {  // "expr" -> assign
        pqlTokenScanner.match(Token::Tag::String);
        pqlTokenScanner.restoreState();
        return parseAssign(pattern);
    } else {}
}

std::unique_ptr<Clause> PatternClauseParser::parseAssign(std::string patternSynonym) {
    if (SemanticValidator::getDesignEntity(synonyms, patternSynonym) != Synonym::DesignEntity::ASSIGN) {
        throw SemanticException();
    }
    pqlTokenScanner.match(Token::Tag::LParen);
    std::unique_ptr<PQLToken> arg1;
    arg1 = parseEntRef();
    pqlTokenScanner.match(Token::Tag::Comma);
    std::unique_ptr<PQLToken> arg2 = parseExpressionSpec();
    pqlTokenScanner.match(Token::Tag::RParen);

    std::unique_ptr<Clause> clause = createClause(std::move(arg1), std::move(arg2), patternSynonym);
    return std::move(clause);
}

std::unique_ptr<Clause> PatternClauseParser::parseWhile(std::string patternSynonym) {
    if (SemanticValidator::getDesignEntity(synonyms, patternSynonym) != Synonym::DesignEntity::WHILE) {
        throw SemanticException();
    }
    pqlTokenScanner.match(Token::Tag::LParen);
    std::unique_ptr<PQLToken> arg1;
    arg1 = parseEntRef();
    pqlTokenScanner.match(Token::Tag::Comma);
    pqlTokenScanner.match(Token::Tag::Underscore);
    pqlTokenScanner.match(Token::Tag::RParen);

    std::unique_ptr<Clause> w = std::make_unique<WhilePattern>(std::move(arg1), patternSynonym);
    return std::move(w);
}

std::unique_ptr<Clause> PatternClauseParser::parseIf(std::string patternSynonym) {
    if (SemanticValidator::getDesignEntity(synonyms, patternSynonym) != Synonym::DesignEntity::IF) {
        throw SemanticException();
    }
    pqlTokenScanner.match(Token::Tag::LParen);
    std::unique_ptr<PQLToken> arg1;
    arg1 = parseEntRef();
    pqlTokenScanner.match(Token::Tag::Comma);
    pqlTokenScanner.match(Token::Tag::Underscore);
    pqlTokenScanner.match(Token::Tag::Comma);
    pqlTokenScanner.match(Token::Tag::Underscore);
    pqlTokenScanner.match(Token::Tag::RParen);

    std::unique_ptr<Clause> i = std::make_unique<IfPattern>(std::move(arg1), patternSynonym);
    return std::move(i);
}

std::unique_ptr<PQLToken> PatternClauseParser::parseEntRef() {
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
    } else {}
}

std::unique_ptr<PQLToken> PatternClauseParser::parseExpressionSpec() {
    if (pqlTokenScanner.peek(Token::Tag::Underscore)) {
        pqlTokenScanner.next();
        if (!pqlTokenScanner.peek(Token::Tag::String)) {
            std::unique_ptr<Wildcard> w = std::make_unique<Wildcard>();
            return std::move(w);
        } else {
            std::unique_ptr<Expression> e = std::make_unique<Expression>(pqlTokenScanner.peekLexeme(), true);
            pqlTokenScanner.next();
            pqlTokenScanner.match(Token::Tag::Underscore);
            return std::move(e);
        }
    } else if (pqlTokenScanner.peek(Token::Tag::String)) {
        std::unique_ptr<Expression> e = std::make_unique<Expression>(pqlTokenScanner.peekLexeme(), false);
        pqlTokenScanner.next();
        pqlTokenScanner.match(Token::Tag::Underscore);
        return std::move(e);
    } else {}
}

std::unique_ptr<Clause> PatternClauseParser::createClause(std::unique_ptr<PQLToken> token1,
                                                          std::unique_ptr<PQLToken> token2,
                                                          const std::string& patternStr) {
    // entRef - variable synonyms, _ , string
    // _ , exact match, partial match
    std::unique_ptr<Clause> a = std::make_unique<AssignPattern>(std::move(token1), std::move(token2), patternStr);
    return std::move(a);
}

Synonym::DesignEntity PatternClauseParser::parsePatternSynonym() {
    std::string synonym = pqlTokenScanner.peekLexeme();
    Synonym::DesignEntity de = SemanticValidator::getDesignEntity(synonyms, synonym);
    pqlTokenScanner.next();
    if (de == Synonym::DesignEntity::ASSIGN || de == Synonym::DesignEntity::WHILE || de == Synonym::DesignEntity::IF) {
        return de;
    }
    throw SemanticException();
}
