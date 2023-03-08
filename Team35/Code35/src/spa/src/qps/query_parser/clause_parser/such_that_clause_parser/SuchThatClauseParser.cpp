#include "SuchThatClauseParser.h"

#include <utility>

#include "qps/pql/StatementNumber.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "qps/pql/Wildcard.h"
#include "qps/clause/UsesModifies.h"
#include "qps/clause/ParentFollows.h"
#include "qps/query_exceptions/SemanticException.h"
#include "qps/pql/Ident.h"

SuchThatClauseParser::SuchThatClauseParser(PQLTokenScanner &pqlTokenScanner, std::vector<Synonym>& synonyms) :
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
    } else {
        throw SyntaxException();
    }
}

std::unique_ptr<Clause> SuchThatClauseParser::parseUsesModifies(std::string& relationship) {
    std::unique_ptr<PQLToken> arg1;
    std::unique_ptr<PQLToken> arg2;
    pqlTokenScanner.matchAndValidate(Token::Tag::LParen);
    if (isStmtRef()) {  // case modifies/usesS
        arg1 = parseStmtRef();
        pqlTokenScanner.matchAndValidate(Token::Tag::Comma);
        arg2 = parseEntRef();
        pqlTokenScanner.matchAndValidate(Token::Tag::RParen);
        relationship += "S";
    } else if (isEntRef()) {  // case modifies/usesP
        arg1 = parseEntRef();
        pqlTokenScanner.matchAndValidate(Token::Tag::Comma);
        arg2 = parseEntRef();
        pqlTokenScanner.matchAndValidate(Token::Tag::RParen);
        relationship += "P";
    } else {
        throw SyntaxException();
    }
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

std::unique_ptr<PQLToken> SuchThatClauseParser::parseEntRef() {
    switch (pqlTokenScanner.peekTag()) {
        case Token::Tag::Underscore: {
            std::unique_ptr<Wildcard> w = std::make_unique<Wildcard>();
            pqlTokenScanner.next();
            return w;
        }
        case Token::Tag::Name: {
            // need semantic validator here
            for (const auto& synonym : synonyms) {
                if (synonym.str() == pqlTokenScanner.peekLexeme()) {
                    pqlTokenScanner.next();
                    std::unique_ptr<Synonym> s = std::make_unique<Synonym>(synonym.de, synonym.str());
                    return std::move(s);
                }
            }
            throw SemanticException();
        }
        case Token::Tag::String: {
            std::unique_ptr<Ident> i = std::make_unique<Ident>(pqlTokenScanner.peekLexeme());
            pqlTokenScanner.next();
            return i;
        }
        default:
            throw SyntaxException();
    }
}

std::unique_ptr<PQLToken> SuchThatClauseParser::parseStmtRef() {
    switch (pqlTokenScanner.peekTag()) {
        case Token::Tag::Underscore: {
            std::unique_ptr<Wildcard> w = std::make_unique<Wildcard>();
            pqlTokenScanner.next();
            return w;
        }
        case Token::Tag::Name: {
            // need semantic validator here
            for (const auto& synonym : synonyms) {
                if (synonym.str() == pqlTokenScanner.peekLexeme()) {
                    pqlTokenScanner.next();
                    std::unique_ptr<Synonym> s = std::make_unique<Synonym>(synonym.de, synonym.str());
                    return std::move(s);
                }
            }
            throw SemanticException();
        }
        case Token::Tag::Integer: {
            std::unique_ptr<StatementNumber> s = std::make_unique<StatementNumber>(stoi(pqlTokenScanner.peekLexeme()));
            pqlTokenScanner.next();
            return s;
        }
        default:
            throw SyntaxException();
    }
}

std::unique_ptr<Clause> SuchThatClauseParser::createClause(std::unique_ptr<PQLToken> token1, std::unique_ptr<PQLToken> token2,
                                     const std::string& relationship) {
    // semantic checking
    // and create
    if (relationship == "ModifiesS") {
        std::unique_ptr<ModifiesS> m = std::make_unique<ModifiesS>(std::move(token1), std::move(token2));
        return std::move(m);
    } else if (relationship == "UsesS") {
        std::unique_ptr<UsesS> u = std::make_unique<UsesS>(std::move(token1), std::move(token2));
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
    }
    throw SyntaxException();
}

bool SuchThatClauseParser::isStmtRef() {
    return pqlTokenScanner.peek(Token::Tag::Integer) || pqlTokenScanner.peek(Token::Tag::Underscore) ||
            pqlTokenScanner.peek(Token::Tag::Name) && isName(pqlTokenScanner.peekLexeme());
}

bool SuchThatClauseParser::isEntRef() {
    return pqlTokenScanner.peek(Token::Tag::Underscore) || (pqlTokenScanner.peek(Token::Tag::Name) && isName(pqlTokenScanner.peekLexeme())) ||
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
