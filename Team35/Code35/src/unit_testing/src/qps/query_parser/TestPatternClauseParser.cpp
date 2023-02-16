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

    Synonym* synonymProcedure = new Synonym(Synonym::DesignEntity::PROCEDURE, "p");
    Synonym* synonymAssign = new Synonym(Synonym::DesignEntity::ASSIGN, "a");
    Synonym* synonymVariable = new Synonym(Synonym::DesignEntity::VARIABLE, "v");
    Synonym* synonymConstant = new Synonym(Synonym::DesignEntity::CONSTANT, "c");

    Ident* ident = new Ident("x");
    Expression* expr = new Expression("x", false);
    Expression* exprWildcardVarName = new Expression("x", true);
    Expression* exprWildcardConstValue = new Expression("1", true);
    Wildcard* wildcard = new Wildcard();

    Clause *clause;
    Pattern *pattern;
    PatternClauseParser pcp;
    std::unique_ptr<ILexer> lexer;

    query = "pattern a(v, _)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator1(lexer);
    clause = pcp.parse(tokenValidator1, declarationList);
    pattern = new Pattern(synonymVariable, wildcard, "a");
    requireTrue(*clause == *pattern);

    query = "pattern a(v,_\"x\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator2(lexer);
    clause = pcp.parse(tokenValidator2, declarationList);
    pattern = new Pattern(synonymVariable, exprWildcardVarName,  "a");
    requireTrue(*clause == *pattern);

    query = "pattern a(v,_\"1\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator3(lexer);
    clause = pcp.parse(tokenValidator3, declarationList);
    pattern = new Pattern(synonymVariable, exprWildcardConstValue, "a");
    requireTrue(*clause == *pattern);

    query = "pattern a(_,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator5(lexer);
    clause = pcp.parse(tokenValidator5, declarationList);
    pattern = new Pattern(wildcard, wildcard, "a");
    requireTrue(*clause == *pattern);

    query = "pattern a(_,_\"x\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator6(lexer);
    clause = pcp.parse(tokenValidator6, declarationList);
    pattern = new Pattern(wildcard, exprWildcardVarName, "a");
    requireTrue(*clause == *pattern);
}
