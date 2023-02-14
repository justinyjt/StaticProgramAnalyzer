#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/query_parser/clause_parser/ClauseParser.h"
#include "commons/lexer/LexerFactory.h"
#include "qps/query_parser/declaration_parser/DeclarationParser.h"
#include "qps/clause/relationship/Modify.h"
#include "qps/pql/StatementNumber.h"
#include "qps/pql/IdentStr.h"
#include "qps/clause/relationship/Uses.h"
#include "qps/clause/relationship/Follows.h"
#include "qps/clause/relationship/Parent.h"
#include "qps/pql/IdentStrWithWildcard.h"

TEST_CASE("SuchThatClauseParser parse(); ModifiesS, (integer, entRef-variable)") {
    std::string query = "such that Modifies(1,v)";
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);

    SuchThatClauseParser stcp;
    std::vector<Synonym> declarationList;
    declarationList.push_back(Synonym(Synonym::DesignEntity::VARIABLE, "v"));
    Clause *clause = stcp.parse(tokenValidator, declarationList);

    StatementNumber* st = new StatementNumber(1);
    Synonym* syn = new Synonym(Synonym::DesignEntity::VARIABLE, "v");
    ModifiesS m(st, syn);
    requireTrue(*clause == m);
}

TEST_CASE("SuchThatClauseParser parse(); ModifiesS, (integer, entRef-IDENT)") {
    std::string query = "such that Modifies(1,\"x\")";
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);

    SuchThatClauseParser stcp;
    std::vector<Synonym> declarationList;
    Clause *clause = stcp.parse(tokenValidator, declarationList);

    StatementNumber* st = new StatementNumber(1);
    IdentStr* identStr = new IdentStr("x");
    ModifiesS m(st, identStr);
    requireTrue(*clause == m);
}

TEST_CASE("SuchThatClauseParser parse(); ModifiesS, (integer, entRef-wildcard)") {
    std::string query = "such that Modifies(1,_)";
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);

    SuchThatClauseParser stcp;
    std::vector<Synonym> declarationList;
    declarationList.push_back(Synonym(Synonym::DesignEntity::VARIABLE, "v"));
    Clause *clause = stcp.parse(tokenValidator, declarationList);

    StatementNumber* st = new StatementNumber(1);
    Wildcard* w = new Wildcard();
    ModifiesS m(st, w);
    requireTrue(*clause == m);
}

TEST_CASE("SuchThatClauseParser parse(); ModifiesP") {
    std::string query = "such that Modifies(c,v)";
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);

    SuchThatClauseParser stcp;
    std::vector<Synonym> declarationList;
    declarationList.push_back(Synonym(Synonym::DesignEntity::CONSTANT, "c"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::VARIABLE, "v"));
    Clause *clause = stcp.parse(tokenValidator, declarationList);

    Synonym* syn1 = new Synonym(Synonym::DesignEntity::CONSTANT, "c");
    Synonym* syn2 = new Synonym(Synonym::DesignEntity::VARIABLE, "v");
    ModifiesP m(syn1, syn2);
    requireTrue(*clause == m);
}

TEST_CASE("SuchThatClauseParser parse(); UsesS, (integer, entRef-variable)") {
    std::string query = "such that Uses(1,v)";
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);

    SuchThatClauseParser stcp;
    std::vector<Synonym> declarationList;
    declarationList.push_back(Synonym(Synonym::DesignEntity::VARIABLE, "v"));
    Clause *clause = stcp.parse(tokenValidator, declarationList);

    StatementNumber* st = new StatementNumber(1);
    Synonym* syn = new Synonym(Synonym::DesignEntity::VARIABLE, "v");
    UsesS u(st, syn);
    requireTrue(*clause == u);
}

TEST_CASE("SuchThatClauseParser parse(); UsesS, (integer, entRef-IDENT)") {
    std::string query = "such that Uses(1,\"x\")";
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);

    SuchThatClauseParser stcp;
    std::vector<Synonym> declarationList;
    Clause *clause = stcp.parse(tokenValidator, declarationList);

    StatementNumber* st = new StatementNumber(1);
    IdentStr* identStr = new IdentStr("x");
    UsesS u(st, identStr);
    requireTrue(*clause == u);
}

TEST_CASE("SuchThatClauseParser parse(); UsesS, (integer, entRef-wildcard)") {
    std::string query = "such that Uses(1,_)";
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);

    SuchThatClauseParser stcp;
    std::vector<Synonym> declarationList;
    declarationList.push_back(Synonym(Synonym::DesignEntity::VARIABLE, "v"));
    Clause *clause = stcp.parse(tokenValidator, declarationList);

    StatementNumber* st = new StatementNumber(1);
    Wildcard* w = new Wildcard();
    UsesS u(st, w);
    requireTrue(*clause == u);
}

TEST_CASE("SuchThatClauseParser parse(); UsesP") {
    std::string query = "such that Uses(s,v)";
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);

    SuchThatClauseParser stcp;
    std::vector<Synonym> declarationList;
    declarationList.push_back(Synonym(Synonym::DesignEntity::CONSTANT, "s"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::VARIABLE, "v"));
    Clause *clause = stcp.parse(tokenValidator, declarationList);

    Synonym* syn1 = new Synonym(Synonym::DesignEntity::CONSTANT, "s");
    Synonym* syn2 = new Synonym(Synonym::DesignEntity::VARIABLE, "v");
    UsesP u(syn1, syn2);
    requireTrue(*clause == u);
}

TEST_CASE("SuchThatClauseParser parse(); Follows") {
    std::string query = "such that Follows(s,v)";
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);

    SuchThatClauseParser stcp;
    std::vector<Synonym> declarationList;
    declarationList.push_back(Synonym(Synonym::DesignEntity::STMT, "s"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::STMT, "v"));
    Clause *clause = stcp.parse(tokenValidator, declarationList);

    Synonym* syn1 = new Synonym(Synonym::DesignEntity::STMT, "s");
    Synonym* syn2 = new Synonym(Synonym::DesignEntity::STMT, "v");
    Follows f(syn1, syn2);
    requireTrue(*clause == f);
}

TEST_CASE("SuchThatClauseParser parse(); FollowsT") {
    std::string query = "such that Follows*(s,v)";
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);

    SuchThatClauseParser stcp;
    std::vector<Synonym> declarationList;
    declarationList.push_back(Synonym(Synonym::DesignEntity::STMT, "s"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::STMT, "v"));
    Clause *clause = stcp.parse(tokenValidator, declarationList);

    Synonym* syn1 = new Synonym(Synonym::DesignEntity::STMT, "s");
    Synonym* syn2 = new Synonym(Synonym::DesignEntity::STMT, "v");
    FollowsT f(syn1, syn2);
    requireTrue(*clause == f);
}

TEST_CASE("SuchThatClauseParser parse(); Parent") {
    std::string query = "such that Parent(s,v)";
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);

    SuchThatClauseParser stcp;
    std::vector<Synonym> declarationList;
    declarationList.push_back(Synonym(Synonym::DesignEntity::STMT, "s"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::STMT, "v"));
    Clause *clause = stcp.parse(tokenValidator, declarationList);

    Synonym* syn1 = new Synonym(Synonym::DesignEntity::STMT, "s");
    Synonym* syn2 = new Synonym(Synonym::DesignEntity::STMT, "v");
    Parent p(syn1, syn2);
    requireTrue(*clause == p);
}

TEST_CASE("SuchThatClauseParser parse(); ParentT") {
    std::string query = "such that Parent*(s,v)";
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);

    SuchThatClauseParser stcp;
    std::vector<Synonym> declarationList;
    declarationList.push_back(Synonym(Synonym::DesignEntity::STMT, "s"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::STMT, "v"));
    Clause *clause = stcp.parse(tokenValidator, declarationList);

    Synonym* syn1 = new Synonym(Synonym::DesignEntity::STMT, "s");
    Synonym* syn2 = new Synonym(Synonym::DesignEntity::STMT, "v");
    ParentT p(syn1, syn2);
    requireTrue(*clause == p);
}

TEST_CASE("SuchThatClauseParser parse(); Pattern, (wildcard, wildcard)") {
    std::string query = "pattern a(_,_)";
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);

    PatternClauseParser pcp;
    std::vector<Synonym> declarationList;
    declarationList.push_back(Synonym(Synonym::DesignEntity::ASSIGN, "a"));
    Clause *clause = pcp.parse(tokenValidator, declarationList);

    Wildcard* w1 = new Wildcard();
    Wildcard* w2 = new Wildcard();
    Pattern a(w1, w2);
    requireTrue(*clause == a);
}

TEST_CASE("SuchThatClauseParser parse(); Pattern, (wildcard, exact match)") {
    std::string query = "pattern a(_,\"x\")";
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);

    PatternClauseParser pcp;
    std::vector<Synonym> declarationList;
    declarationList.push_back(Synonym(Synonym::DesignEntity::ASSIGN, "a"));
    Clause *clause = pcp.parse(tokenValidator, declarationList);

    Wildcard* w1 = new Wildcard();
    IdentStr* w2 = new IdentStr("x");
    Pattern a(w1, w2);
    requireTrue(*clause == a);
}

TEST_CASE("SuchThatClauseParser parse(); Pattern, (wildcard, partial match)") {
    std::string query = "pattern a(_,_\"x\"_)";
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);

    PatternClauseParser pcp;
    std::vector<Synonym> declarationList;
    declarationList.push_back(Synonym(Synonym::DesignEntity::ASSIGN, "a"));
    Clause *clause = pcp.parse(tokenValidator, declarationList);

    Wildcard* w1 = new Wildcard();
    IdentStrWithWildcard* w2 = new IdentStrWithWildcard("x");
    Pattern a(w1, w2);
    requireTrue(*clause == a);
}
