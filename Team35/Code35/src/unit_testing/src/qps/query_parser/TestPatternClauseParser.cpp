#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/pql/Synonym.h"
#include "qps/clause/Clause.h"
#include "qps/query_parser/clause_parser/pattern_clause_parser/PatternClauseParser.h"
#include "commons/lexer/LexerFactory.h"
#include "qps/pql/Ident.h"
#include "qps/pql/Expression.h"

TEST_CASE("PatternClauseParser parse(); assign, test for semantic error") {
    std::string query = "";
    std::vector<Synonym> declarationList;
    declarationList.push_back(Synonym(Synonym::DesignEntity::PROCEDURE, "p"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::ASSIGN, "a"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::VARIABLE, "v"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::CONSTANT, "c"));

    std::shared_ptr<Synonym> synonymProcedure = std::make_shared<Synonym>(Synonym::DesignEntity::PROCEDURE, "p");
    std::shared_ptr<Synonym> synonymAssign = std::make_shared<Synonym>(Synonym::DesignEntity::ASSIGN, "a");
    std::shared_ptr<Synonym> synonymVariable = std::make_shared<Synonym>(Synonym::DesignEntity::VARIABLE, "v");
    std::shared_ptr<Synonym> synonymConstant = std::make_shared<Synonym>(Synonym::DesignEntity::CONSTANT, "c");

    std::shared_ptr<Ident> ident = std::make_shared<Ident>("x");
    std::shared_ptr<Expression> expr = std::make_shared<Expression>("x", false);
    std::shared_ptr<Expression> exprWildcardVarName = std::make_shared<Expression>("x", true);
    std::shared_ptr<Expression> exprWildcardConstValue = std::make_shared<Expression>("1", true);
    std::shared_ptr<Wildcard> wildcard = std::make_shared<Wildcard>();

    std::unique_ptr<Clause> clause;
    std::unique_ptr<Pattern> pattern;
    PatternClauseParser pcp;
    std::unique_ptr<ILexer> lexer;

    query = "pattern a(v, _)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator1(lexer);
    clause = pcp.parse(tokenValidator1, declarationList);
    pattern = std::make_unique<Pattern>(synonymVariable, wildcard, "a");
    requireTrue(*clause == *pattern);

    query = "pattern a(v,_\"x\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator2(lexer);
    clause = pcp.parse(tokenValidator2, declarationList);
    pattern = std::make_unique<Pattern>(synonymVariable, exprWildcardVarName,  "a");
    requireTrue(*clause == *pattern);

    query = "pattern a(v,_\"1\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator3(lexer);
    clause = pcp.parse(tokenValidator3, declarationList);
    pattern = std::make_unique<Pattern>(synonymVariable, exprWildcardConstValue, "a");
    requireTrue(*clause == *pattern);

    query = "pattern a(_,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator5(lexer);
    clause = pcp.parse(tokenValidator5, declarationList);
    pattern = std::make_unique<Pattern>(wildcard, wildcard, "a");
    requireTrue(*clause == *pattern);

    query = "pattern a(_,_\"x\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator6(lexer);
    clause = pcp.parse(tokenValidator6, declarationList);
    pattern = std::make_unique<Pattern>(wildcard, exprWildcardVarName, "a");
    requireTrue(*clause == *pattern);
}
