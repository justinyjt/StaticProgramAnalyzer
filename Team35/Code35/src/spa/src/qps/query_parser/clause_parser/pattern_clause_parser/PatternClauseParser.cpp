#include <string>
#include "PatternClauseParser.h"
#include "qps/clause/two_arg_clause/AssignPattern.h"
#include "qps/pql/Synonym.h"
#include "qps/pql/Wildcard.h"
#include "qps/pql/Expression.h"
#include "qps/pql/Ident.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "qps/query_exceptions/SemanticException.h"
#include "commons/token_scanner/TokenScanner.h"
#include "qps/query_parser/SemanticValidator.h"
#include "qps/clause/one_arg_clause/OneArgClause.h"

PatternClauseParser::PatternClauseParser(PQLTokenScanner& pqlTokenScanner,
                                         std::unordered_map<std::string, Synonym::DesignEntity>& synonyms) :
        pqlTokenScanner(pqlTokenScanner), synonyms(synonyms) {}

std::unique_ptr<Clause> PatternClauseParser::parse() {
    pqlTokenScanner.match(Token::Tag::Pattern);
    return std::move(parsePattern());
}

std::unique_ptr<Clause> PatternClauseParser::parsePattern() {
    // validate and return type
    Synonym::DesignEntity de;
    std::string pattern = pqlTokenScanner.peekLexeme();
    if (pqlTokenScanner.isName()) {
        de = parsePatternSynonym();
    } else {
        throw SyntaxException();
    }

    // if pattern is ASSIGN, call parseAssign
    switch (de) {
        case Synonym::DesignEntity::ASSIGN:
            return parseAssign(pattern);
        case Synonym::DesignEntity::WHILE:
            return parseWhile(pattern);
        case Synonym::DesignEntity::IF:
            return parseIf(pattern);
        default:
            {}
    }
}

std::unique_ptr<Clause> PatternClauseParser::parseAssign(std::string patternSynonym) {
    pqlTokenScanner.matchAndValidate(Token::Tag::LParen);
    std::unique_ptr<PQLToken> arg1;
    arg1 = parseEntRef();
    pqlTokenScanner.matchAndValidate(Token::Tag::Comma);
    std::unique_ptr<PQLToken> arg2 = parseExpressionSpec();
    pqlTokenScanner.matchAndValidate(Token::Tag::RParen);

    std::unique_ptr<Clause> clause = createClause(std::move(arg1), std::move(arg2), patternSynonym);
    return std::move(clause);
}

std::unique_ptr<Clause> PatternClauseParser::parseWhile(std::string patternSynonym) {
    pqlTokenScanner.matchAndValidate(Token::Tag::LParen);
    std::unique_ptr<PQLToken> arg1;
    arg1 = parseEntRef();
    pqlTokenScanner.matchAndValidate(Token::Tag::Comma);
    pqlTokenScanner.matchAndValidate(Token::Tag::Underscore);
    pqlTokenScanner.matchAndValidate(Token::Tag::RParen);

    std::unique_ptr<Clause> w = std::make_unique<WhilePattern>(std::move(arg1), patternSynonym);
    return std::move(w);
}

std::unique_ptr<Clause> PatternClauseParser::parseIf(std::string patternSynonym) {
    pqlTokenScanner.matchAndValidate(Token::Tag::LParen);
    std::unique_ptr<PQLToken> arg1;
    arg1 = parseEntRef();
    pqlTokenScanner.matchAndValidate(Token::Tag::Comma);
    pqlTokenScanner.matchAndValidate(Token::Tag::Underscore);
    pqlTokenScanner.matchAndValidate(Token::Tag::Comma);
    pqlTokenScanner.matchAndValidate(Token::Tag::Underscore);
    pqlTokenScanner.matchAndValidate(Token::Tag::RParen);

    std::unique_ptr<Clause> i = std::make_unique<IfPattern>(std::move(arg1), patternSynonym);
    return std::move(i);
}

std::unique_ptr<PQLToken> PatternClauseParser::parseEntRef() {
    if (!pqlTokenScanner.peekEntRef()) {
        throw SyntaxException();
    }
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
            if (isValidExpr()) {
                std::unique_ptr<Expression> e = std::make_unique<Expression>(pqlTokenScanner.peekLexeme(), true);
                pqlTokenScanner.next();
                pqlTokenScanner.matchAndValidate(Token::Tag::Underscore);
                return std::move(e);
            }
            throw SyntaxException();
        }
    } else if (pqlTokenScanner.peek(Token::Tag::String)) {
        if (isValidExpr()) {
            std::unique_ptr<Expression> e = std::make_unique<Expression>(pqlTokenScanner.peekLexeme(), false);
            pqlTokenScanner.next();
            return std::move(e);
        }
        throw SyntaxException();
    } else {
        throw SyntaxException();
    }
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

bool PatternClauseParser::isValidExpr() {
    return pqlTokenScanner.peek(Token::Tag::String) &&
                   pqlTokenScanner.peekIdent() || pqlTokenScanner.peekConstant();
}
