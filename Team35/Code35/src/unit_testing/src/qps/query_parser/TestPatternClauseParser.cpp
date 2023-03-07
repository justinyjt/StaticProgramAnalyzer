#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/pql/Synonym.h"
#include "qps/clause/Clause.h"
#include "qps/query_parser/clause_parser/pattern_clause_parser/PatternClauseParser.h"
#include "commons/lexer/LexerFactory.h"
#include "qps/pql/Ident.h"
#include "qps/pql/Expression.h"
#include "qps/query_exceptions/SyntaxException.h"

class setUp {
public:
    setUp() {
        // Set up code
        query = "";
        declarationList.emplace_back(Synonym::DesignEntity::PROCEDURE, "p");
        declarationList.emplace_back(Synonym::DesignEntity::ASSIGN, "a");
        declarationList.emplace_back(Synonym::DesignEntity::VARIABLE, "v");
        declarationList.emplace_back(Synonym::DesignEntity::CONSTANT, "c");

        synonymProcedure = std::make_unique<Synonym>(Synonym::DesignEntity::PROCEDURE, "p");
        synonymAssign = std::make_unique<Synonym>(Synonym::DesignEntity::ASSIGN, "a");
        synonymVariable = std::make_unique<Synonym>(Synonym::DesignEntity::VARIABLE, "v");
        synonymConstant = std::make_unique<Synonym>(Synonym::DesignEntity::CONSTANT, "c");

        ident = std::make_unique<Ident>("x");
        expr = std::make_unique<Expression>("x", false);
        exprWildcardVarName = std::make_unique<Expression>("x", true);
        exprWildcardConstValue = std::make_unique<Expression>("1", true);
        wildcard1 = std::make_unique<Wildcard>();
        wildcard2 = std::make_unique<Wildcard>();
    }

    ~setUp() {
        // Tear down code
    }

    // Declarations
    std::string query;
    std::vector<Synonym> declarationList;

    std::unique_ptr<Synonym> synonymProcedure;
    std::unique_ptr<Synonym> synonymAssign;
    std::unique_ptr<Synonym> synonymVariable;
    std::unique_ptr<Synonym> synonymConstant;

    std::unique_ptr<Ident> ident;
    std::unique_ptr<Expression> expr;
    std::unique_ptr<Expression> exprWildcardVarName;
    std::unique_ptr<Expression> exprWildcardConstValue;
    std::unique_ptr<Wildcard> wildcard1;
    std::unique_ptr<Wildcard> wildcard2;

    std::unique_ptr<Clause> clause;
    std::unique_ptr<Pattern> pattern;
    std::unique_ptr<PatternClauseParser> pcp;
    std::unique_ptr<ILexer> lexer;
};

TEST_CASE_METHOD(setUp, "variable, wildcard") {
    query = "pattern a(v, _)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator1(lexer);
    clause = pcp->parse(tokenValidator1, declarationList);
    pattern = std::make_unique<Pattern>(std::move(synonymVariable), std::move(wildcard1), "a");
    requireTrue(*clause == *pattern);
}

TEST_CASE_METHOD(setUp, "variable, variable name and wildcard") {
    query = "pattern a(v,_\"x\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator2(lexer);
    clause = pcp->parse(tokenValidator2, declarationList);
    pattern = std::make_unique<Pattern>(std::move(synonymVariable), std::move(exprWildcardVarName),  "a");
    requireTrue(*clause == *pattern);
}

TEST_CASE_METHOD(setUp, "variable, constant value and wildcard") {
    query = "pattern a(v,_\"1\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator3(lexer);
    clause = pcp->parse(tokenValidator3, declarationList);
    pattern = std::make_unique<Pattern>(std::move(synonymVariable), std::move(exprWildcardConstValue), "a");
    requireTrue(*clause == *pattern);
}

TEST_CASE_METHOD(setUp, "wildcard and wildcard") {
    query = "pattern a(_,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator5(lexer);
    clause = pcp->parse(tokenValidator5, declarationList);
    pattern = std::make_unique<Pattern>(std::move(wildcard1), std::move(wildcard2), "a");
    requireTrue(*clause == *pattern);
}

TEST_CASE_METHOD(setUp, "wildcard and variable name with wildcard") {
    query = "pattern a(_,_\"x\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator6(lexer);
    clause = pcp->parse(tokenValidator6, declarationList);
    pattern = std::make_unique<Pattern>(std::move(wildcard1), std::move(exprWildcardVarName), "a");
    requireTrue(*clause == *pattern);
}

TEST_CASE_METHOD(setUp, "invalid LHS constant and variable name with wildcard") {
    query = "pattern a(\"1\",_\"x\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator7(lexer);
    requireThrowAs<SyntaxException>([&tokenValidator7]() {
        std::vector<Synonym> dl = setUp().declarationList;
        setUp().pcp->parse(tokenValidator7, dl);
    });
}

TEST_CASE_METHOD(setUp, "invalid LHS varName and variable name with wildcard") {
    query = "pattern a(\"+x\",_\"x\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator8(lexer);
    requireThrowAs<SyntaxException>([&tokenValidator8]() {
        std::vector<Synonym> dl = setUp().declarationList;
        setUp().pcp->parse(tokenValidator8, dl);
    });
}

TEST_CASE_METHOD(setUp, "ident and invalid RHS expr 1") {
    query = "pattern a(\"x\",_\"+x\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator9(lexer);
    requireThrowAs<SyntaxException>([&tokenValidator9]() {
        std::vector<Synonym> dl = setUp().declarationList;
        setUp().pcp->parse(tokenValidator9, dl);
    });
}

TEST_CASE_METHOD(setUp, "ident and invalid RHS expr 2") {
    query = "pattern a(\"x\",_\"x+\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator10(lexer);
    requireThrowAs<SyntaxException>([&tokenValidator10]() {
        std::vector<Synonym> dl = setUp().declarationList;
        setUp().pcp->parse(tokenValidator10, dl);
    });
}

