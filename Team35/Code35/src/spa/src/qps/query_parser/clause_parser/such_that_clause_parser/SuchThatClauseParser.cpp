#include "SuchThatClauseParser.h"

#include <utility>

#include "qps/pql/StatementNumber.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "qps/pql/Wildcard.h"
#include "qps/clause/TwoArgClause/StmtEntClause.h"
#include "qps/clause/TwoArgClause/StmtStmtClause.h"
#include "qps/pql/Ident.h"
#include "qps/query_parser/SemanticValidator.h"
#include "qps/clause/EntEntClause.h"

SuchThatClauseParser::SuchThatClauseParser(PQLTokenScanner& pqlTokenScanner, std::unordered_map<std::string, Synonym::DesignEntity>& synonyms) :
    pqlTokenScanner(pqlTokenScanner), synonyms(synonyms) {}

std::unique_ptr<Clause> SuchThatClauseParser::parse() {
    pqlTokenScanner.matchAndValidate(Token::Tag::Such);
    pqlTokenScanner.matchAndValidate(Token::Tag::That);

    std::unique_ptr<Clause> clause = parseRelationship();

    return clause;
}

std::unique_ptr<Clause> SuchThatClauseParser::parseRelationship() {
    std::string relationship;
    if (pqlTokenScanner.peek(Token::Tag::Modifies) || pqlTokenScanner.peek(Token::Tag::Uses)) {
        relationship += pqlTokenScanner.peekLexeme();
        pqlTokenScanner.next();
        return std::move(parseUsesModifies(relationship));
        // is stmtRef, entRef or entRef, entRef
    } else if (pqlTokenScanner.peek(Token::Tag::Parent) || pqlTokenScanner.peek(Token::Tag::Follows)) {
        // is stmtRef, stmtRef
        relationship += pqlTokenScanner.peekLexeme();
        pqlTokenScanner.next();
        if (pqlTokenScanner.peek(Token::Tag::Star)) {
            relationship += "T";
            pqlTokenScanner.next();
        }
        return std::move(parseStmtStmt(relationship));
    } else if (pqlTokenScanner.peek(Token::Tag::Calls)) {
        relationship += pqlTokenScanner.peekLexeme();
        pqlTokenScanner.next();
        if (pqlTokenScanner.peek(Token::Tag::Star)) {
            relationship += "T";
            pqlTokenScanner.next();
        }
        return std::move(parseEntEnt(relationship));
    } else {
        throw SyntaxException();
    }
}

std::unique_ptr<Clause> SuchThatClauseParser::parseUsesModifies(std::string& relationship) {
    std::unique_ptr<PQLToken> arg1;
    std::unique_ptr<PQLToken> arg2;
    pqlTokenScanner.matchAndValidate(Token::Tag::LParen);
    if (isStmtRef() || isEntRef()) {

        // decide S or P
        switch (pqlTokenScanner.peekTag()) {
            case Token::Tag::Underscore: {
                arg1 = std::make_unique<Wildcard>();
                relationship += "S";
                pqlTokenScanner.next();
                break;
            }
            case Token::Tag::Name: {
                // need semantic validator here
                std::string synonym = pqlTokenScanner.peekLexeme();
                Synonym::DesignEntity de = SemanticValidator::getDesignEntity(synonyms, synonym);
                arg1 = std::make_unique<Synonym>(de, synonym);
                if (de == Synonym::DesignEntity::STMT || de == Synonym::DesignEntity::READ ||
                    de == Synonym::DesignEntity::PRINT || de == Synonym::DesignEntity::ASSIGN ||
                    de == Synonym::DesignEntity::CALL || de == Synonym::DesignEntity::WHILE ||
                    de == Synonym::DesignEntity::IF) {
                    relationship += "S";
                } else {
                    relationship += "P";
                }
                pqlTokenScanner.next();
                break;
            }
            case Token::Tag::String: {
                arg1 = std::make_unique<Ident>(pqlTokenScanner.peekLexeme());
                relationship += "P";
                pqlTokenScanner.next();
                break;
            }
            case Token::Tag::Integer: {
                arg1 = std::make_unique<StatementNumber>(
                        stoi(pqlTokenScanner.peekLexeme()));
                relationship += "S";
                pqlTokenScanner.next();
                break;
            }
            default:
                throw SyntaxException();
        }
    }

    pqlTokenScanner.matchAndValidate(Token::Tag::Comma);
    arg2 = parseEntRef();
    pqlTokenScanner.matchAndValidate(Token::Tag::RParen);

    return std::move(createClause(std::move(arg1), std::move(arg2), relationship));
}

std::unique_ptr<Clause> SuchThatClauseParser::parseStmtStmt(std::string& relationship) {
    std::unique_ptr<PQLToken> arg1;
    std::unique_ptr<PQLToken> arg2;

    pqlTokenScanner.matchAndValidate(Token::Tag::LParen);
    arg1 = parseStmtRef();

    pqlTokenScanner.matchAndValidate(Token::Tag::Comma);
    arg2 = parseStmtRef();

    pqlTokenScanner.matchAndValidate(Token::Tag::RParen);
    return std::move(createClause(std::move(arg1), std::move(arg2), relationship));
}

std::unique_ptr<Clause> SuchThatClauseParser::parseEntEnt(std::string& relationship) {
    std::unique_ptr<PQLToken> arg1;
    std::unique_ptr<PQLToken> arg2;

    pqlTokenScanner.matchAndValidate(Token::Tag::LParen);
    arg1 = parseEntRef();

    pqlTokenScanner.matchAndValidate(Token::Tag::Comma);
    arg2 = parseEntRef();

    pqlTokenScanner.matchAndValidate(Token::Tag::RParen);
    return std::move(createClause(std::move(arg1), std::move(arg2), relationship));
}

std::unique_ptr<PQLToken> SuchThatClauseParser::parseEntRef() {
    if (!isEntRef()) {
        throw SyntaxException();
    }
    switch (pqlTokenScanner.peekTag()) {
        case Token::Tag::Underscore: {
            std::unique_ptr<Wildcard> w = std::make_unique<Wildcard>();
            pqlTokenScanner.next();
            return w;
        }
        case Token::Tag::Name: case Token::Tag::Bool: {
            std::string synonym = pqlTokenScanner.peekLexeme();
            Synonym::DesignEntity de = SemanticValidator::getDesignEntity(synonyms, synonym);
            pqlTokenScanner.next();
            std::unique_ptr<Synonym> s = std::make_unique<Synonym>(de, synonym);
            return std::move(s);
        }
        case Token::Tag::String: {
            std::unique_ptr<Ident> i = std::make_unique<Ident>(pqlTokenScanner.peekLexeme());
            pqlTokenScanner.next();
            return i;
        }
        default:
            assert(false);
    }
}

std::unique_ptr<PQLToken> SuchThatClauseParser::parseStmtRef() {
    if (!isStmtRef()) {
        throw SyntaxException();
    }
    switch (pqlTokenScanner.peekTag()) {
        case Token::Tag::Underscore: {
            std::unique_ptr<Wildcard> w = std::make_unique<Wildcard>();
            pqlTokenScanner.next();
            return w;
        }
        case Token::Tag::Name: case Token::Tag::Bool: {
            // need semantic validator here
            std::string synonym = pqlTokenScanner.peekLexeme();
            Synonym::DesignEntity de = SemanticValidator::getDesignEntity(synonyms, synonym);
            pqlTokenScanner.next();
            std::unique_ptr<Synonym> s = std::make_unique<Synonym>(de, synonym);
            return std::move(s);
        }
        case Token::Tag::Integer: {
            std::unique_ptr<StatementNumber> s = std::make_unique<StatementNumber>(stoi(pqlTokenScanner.peekLexeme()));
            pqlTokenScanner.next();
            return s;
        }
        default:
            assert(false);
    }
}

std::unique_ptr<Clause> SuchThatClauseParser::createClause(std::unique_ptr<PQLToken> token1, std::unique_ptr<PQLToken> token2,
                                     const std::string& relationship) {
    // semantic checking
    // and create
    if (relationship == "ModifiesS") {
        std::unique_ptr<ModifiesS> m = std::make_unique<ModifiesS>(std::move(token1), std::move(token2));
        return std::move(m);
    } else if (relationship == "ModifiesP") {
        std::unique_ptr<ModifiesP> m = std::make_unique<ModifiesP>(std::move(token1), std::move(token2));
        return std::move(m);
    } else if (relationship == "UsesS") {
        std::unique_ptr<UsesS> u = std::make_unique<UsesS>(std::move(token1), std::move(token2));
        return std::move(u);
    } else if (relationship == "UsesP") {
        std::unique_ptr<UsesP> u = std::make_unique<UsesP>(std::move(token1), std::move(token2));
        return std::move(u);
    } else if (relationship == "Follows") {
        std::unique_ptr<Follows> f = std::make_unique<Follows>(std::move(token1), std::move(token2), false);
        return std::move(f);
    } else if (relationship == "FollowsT") {
        std::unique_ptr<Follows> f = std::make_unique<Follows>(std::move(token1), std::move(token2), true);
        return std::move(f);
    } else if (relationship == "Parent") {
        std::unique_ptr<Parent> p = std::make_unique<Parent>(std::move(token1), std::move(token2), false);
        return std::move(p);
    } else if (relationship == "ParentT") {
        std::unique_ptr<Parent> p = std::make_unique<Parent>(std::move(token1), std::move(token2), true);
        return std::move(p);
    } else if (relationship == "Calls") {
        std::unique_ptr<Calls> p = std::make_unique<Calls>(std::move(token1), std::move(token2), false);
        return std::move(p);
    } else if (relationship == "CallsT") {
        std::unique_ptr<Calls> p = std::make_unique<Calls>(std::move(token1), std::move(token2), true);
        return std::move(p);
    }
    throw SyntaxException();
}

bool SuchThatClauseParser::isStmtRef() {
    return pqlTokenScanner.peek(Token::Tag::Integer) || pqlTokenScanner.peek(Token::Tag::Underscore) ||
            isName(pqlTokenScanner.peekLexeme());
}

bool SuchThatClauseParser::isEntRef() {
    return pqlTokenScanner.peek(Token::Tag::Underscore) || isName(pqlTokenScanner.peekLexeme()) ||
           pqlTokenScanner.peek(Token::Tag::String) && isName(pqlTokenScanner.peekLexeme());
}

bool SuchThatClauseParser::isName(std::string input) {
    if (!isalpha(input[0])) {
        return false;
    }
    for (char c : input) {
        if (!isalpha(c) && !isdigit(c)) {
            return false;
        }
    }
    return true;
}
