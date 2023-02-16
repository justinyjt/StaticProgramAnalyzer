#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/query_parser/clause_parser/ClauseParser.h"
#include "commons/lexer/LexerFactory.h"
#include "qps/query_parser/declaration_parser/DeclarationParser.h"
#include "qps/pql/StatementNumber.h"
#include "qps/clause/Modifies.h"
#include "qps/pql/Ident.h"
#include "qps/clause/Uses.h"
#include "qps/clause/Follows.h"
#include "qps/clause/Parent.h"

TEST_CASE("SuchThatClauseParser parse(); ModifiesS, argument type test") {
    std::string query = "";
    std::vector<Synonym> declarationList;
    declarationList.push_back(Synonym(Synonym::DesignEntity::PROCEDURE, "p"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::STMT, "s"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::READ, "r"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::PRINT, "pn"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::ASSIGN, "a"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::CALL, "cl"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::WHILE, "w"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::IF, "ifs"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::VARIABLE, "v"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::CONSTANT, "c"));

    Synonym* synonymProcedure = new Synonym(Synonym::DesignEntity::PROCEDURE, "p");
    Synonym* synonymStatement = new Synonym(Synonym::DesignEntity::STMT, "s");
    Synonym* synonymRead = new Synonym(Synonym::DesignEntity::READ, "r");
    Synonym* synonymPrint = new Synonym(Synonym::DesignEntity::PRINT, "pn");
    Synonym* synonymAssign = new Synonym(Synonym::DesignEntity::ASSIGN, "a");
    Synonym* synonymCall = new Synonym(Synonym::DesignEntity::CALL, "cl");
    Synonym* synonymWhile = new Synonym(Synonym::DesignEntity::WHILE, "w");
    Synonym* synonymIf = new Synonym(Synonym::DesignEntity::IF, "ifs");
    Synonym* synonymVariable = new Synonym(Synonym::DesignEntity::VARIABLE, "v");
    Synonym* synonymConstant = new Synonym(Synonym::DesignEntity::CONSTANT, "c");

    Ident* identStr = new Ident("x");
    StatementNumber* statementNumber = new StatementNumber(1);
    Wildcard* wildcard = new Wildcard();

    Clause *clause;
    ModifiesS *modifiesS;
    SuchThatClauseParser stcp;
    std::unique_ptr<ILexer> lexer;

    query = "such that Modifies(p,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator1(lexer);
    requireThrow([&tokenValidator1, &stcp, &declarationList]() {
        stcp.parse(tokenValidator1, declarationList);
    });

    query = "such that Modifies(s,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator2(lexer);
    clause = stcp.parse(tokenValidator2, declarationList);
    modifiesS = new ModifiesS(synonymStatement, synonymVariable);
    requireTrue(*clause == *modifiesS);

    query = "such that Modifies(r,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator3(lexer);
    clause = stcp.parse(tokenValidator3, declarationList);
    modifiesS = new ModifiesS(synonymRead, synonymVariable);
    requireTrue(*clause == *modifiesS);

    query = "such that Modifies(pn,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator4(lexer);
    requireThrow([&tokenValidator4, &stcp, &declarationList]() {
        stcp.parse(tokenValidator4, declarationList);
    });

    query = "such that Modifies(a,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator5(lexer);
    clause = stcp.parse(tokenValidator5, declarationList);
    modifiesS = new ModifiesS(synonymAssign, synonymVariable);
    requireTrue(*clause == *modifiesS);

    query = "such that Modifies(cl,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator6(lexer);
    clause = stcp.parse(tokenValidator6, declarationList);
    modifiesS = new ModifiesS(synonymCall, synonymVariable);
    requireTrue(*clause == *modifiesS);

    query = "such that Modifies(w,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator7(lexer);
    clause = stcp.parse(tokenValidator7, declarationList);
    modifiesS = new ModifiesS(synonymWhile, synonymVariable);
    requireTrue(*clause == *modifiesS);

    query = "such that Modifies(ifs,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator8(lexer);
    clause = stcp.parse(tokenValidator8, declarationList);
    modifiesS = new ModifiesS(synonymIf, synonymVariable);
    requireTrue(*clause == *modifiesS);



    query = "such that Modifies(s,\"x\")";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator9(lexer);
    clause = stcp.parse(tokenValidator9, declarationList);
    modifiesS = new ModifiesS(synonymStatement, identStr);
    requireTrue(*clause == *modifiesS);

    query = "such that Modifies(s,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator10(lexer);
    clause = stcp.parse(tokenValidator10, declarationList);
    modifiesS = new ModifiesS(synonymStatement, wildcard);
    requireTrue(*clause == *modifiesS);



    query = "such that Modifies(1,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator11(lexer);
    clause = stcp.parse(tokenValidator11, declarationList);
    modifiesS = new ModifiesS(statementNumber, synonymVariable);
    requireTrue(*clause == *modifiesS);

    query = "such that Modifies(1,\"x\")";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator12(lexer);
    clause = stcp.parse(tokenValidator12, declarationList);
    modifiesS = new ModifiesS(statementNumber, identStr);
    requireTrue(*clause == *modifiesS);

    query = "such that Modifies(1,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator13(lexer);
    clause = stcp.parse(tokenValidator13, declarationList);
    modifiesS = new ModifiesS(statementNumber, wildcard);
    requireTrue(*clause == *modifiesS);


    query = "such that Modifies(_,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator14(lexer);
    requireThrow([&tokenValidator14, &stcp, &declarationList]() {
        stcp.parse(tokenValidator14, declarationList);
    });
}

TEST_CASE("SuchThatClauseParser parse(); UsesS, argument type test") {
    std::string query = "";
    std::vector<Synonym> declarationList;
    declarationList.push_back(Synonym(Synonym::DesignEntity::PROCEDURE, "p"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::STMT, "s"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::READ, "r"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::PRINT, "pn"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::ASSIGN, "a"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::CALL, "cl"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::WHILE, "w"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::IF, "ifs"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::VARIABLE, "v"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::CONSTANT, "c"));

    Synonym* synonymProcedure = new Synonym(Synonym::DesignEntity::PROCEDURE, "p");
    Synonym* synonymStatement = new Synonym(Synonym::DesignEntity::STMT, "s");
    Synonym* synonymRead = new Synonym(Synonym::DesignEntity::READ, "r");
    Synonym* synonymPrint = new Synonym(Synonym::DesignEntity::PRINT, "pn");
    Synonym* synonymAssign = new Synonym(Synonym::DesignEntity::ASSIGN, "a");
    Synonym* synonymCall = new Synonym(Synonym::DesignEntity::CALL, "cl");
    Synonym* synonymWhile = new Synonym(Synonym::DesignEntity::WHILE, "w");
    Synonym* synonymIf = new Synonym(Synonym::DesignEntity::IF, "ifs");
    Synonym* synonymVariable = new Synonym(Synonym::DesignEntity::VARIABLE, "v");
    Synonym* synonymConstant = new Synonym(Synonym::DesignEntity::CONSTANT, "c");

    Ident* identStr = new Ident("x");
    StatementNumber* statementNumber = new StatementNumber(1);
    Wildcard* wildcard = new Wildcard();

    Clause *clause;
    UsesS *usesS;
    SuchThatClauseParser stcp;
    std::unique_ptr<ILexer> lexer;

    query = "such that Uses(p,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator1(lexer);
    requireThrow([&tokenValidator1, &stcp, &declarationList]() {
        stcp.parse(tokenValidator1, declarationList);
    });

    query = "such that Uses(s,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator2(lexer);
    clause = stcp.parse(tokenValidator2, declarationList);
    usesS = new UsesS(synonymStatement, synonymVariable);
    requireTrue(*clause == *usesS);

    query = "such that Uses(pn,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator3(lexer);
    clause = stcp.parse(tokenValidator3, declarationList);
    usesS = new UsesS(synonymPrint, synonymVariable);
    requireTrue(*clause == *usesS);

    query = "such that Uses(r,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator4(lexer);
    requireThrow([&tokenValidator4, &stcp, &declarationList]() {
        stcp.parse(tokenValidator4, declarationList);
    });

    query = "such that Uses(a,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator5(lexer);
    clause = stcp.parse(tokenValidator5, declarationList);
    usesS = new UsesS(synonymAssign, synonymVariable);
    requireTrue(*clause == *usesS);

    query = "such that Uses(cl,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator6(lexer);
    clause = stcp.parse(tokenValidator6, declarationList);
    usesS = new UsesS(synonymCall, synonymVariable);
    requireTrue(*clause == *usesS);

    query = "such that Uses(w,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator7(lexer);
    clause = stcp.parse(tokenValidator7, declarationList);
    usesS = new UsesS(synonymWhile, synonymVariable);
    requireTrue(*clause == *usesS);

    query = "such that Uses(ifs,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator8(lexer);
    clause = stcp.parse(tokenValidator8, declarationList);
    usesS = new UsesS(synonymIf, synonymVariable);
    requireTrue(*clause == *usesS);



    query = "such that Uses(s,\"x\")";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator9(lexer);
    clause = stcp.parse(tokenValidator9, declarationList);
    usesS = new UsesS(synonymStatement, identStr);
    requireTrue(*clause == *usesS);

    query = "such that Uses(s,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator10(lexer);
    clause = stcp.parse(tokenValidator10, declarationList);
    usesS = new UsesS(synonymStatement, wildcard);
    requireTrue(*clause == *usesS);



    query = "such that Uses(1,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator11(lexer);
    clause = stcp.parse(tokenValidator11, declarationList);
    usesS = new UsesS(statementNumber, synonymVariable);
    requireTrue(*clause == *usesS);

    query = "such that Uses(1,\"x\")";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator12(lexer);
    clause = stcp.parse(tokenValidator12, declarationList);
    usesS = new UsesS(statementNumber, identStr);
    requireTrue(*clause == *usesS);

    query = "such that Uses(1,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator13(lexer);
    clause = stcp.parse(tokenValidator13, declarationList);
    usesS = new UsesS(statementNumber, wildcard);
    requireTrue(*clause == *usesS);


    query = "such that Uses(_,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator14(lexer);
    requireThrow([&tokenValidator14, &stcp, &declarationList]() {
        stcp.parse(tokenValidator14, declarationList);
    });
}

TEST_CASE("SuchThatClauseParser parse(); Follows, argument type test") {
    std::string query = "";
    std::vector<Synonym> declarationList;
    declarationList.push_back(Synonym(Synonym::DesignEntity::PROCEDURE, "p"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::STMT, "s"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::STMT, "s1"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::READ, "r"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::PRINT, "pn"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::ASSIGN, "a"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::CALL, "cl"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::WHILE, "w"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::IF, "ifs"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::VARIABLE, "v"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::CONSTANT, "c"));

    Synonym* synonymProcedure = new Synonym(Synonym::DesignEntity::PROCEDURE, "p");
    Synonym* synonymStatement = new Synonym(Synonym::DesignEntity::STMT, "s");
    Synonym* synonymStatement1 = new Synonym(Synonym::DesignEntity::STMT, "s1");
    Synonym* synonymRead = new Synonym(Synonym::DesignEntity::READ, "r");
    Synonym* synonymPrint = new Synonym(Synonym::DesignEntity::PRINT, "pn");
    Synonym* synonymAssign = new Synonym(Synonym::DesignEntity::ASSIGN, "a");
    Synonym* synonymCall = new Synonym(Synonym::DesignEntity::CALL, "cl");
    Synonym* synonymWhile = new Synonym(Synonym::DesignEntity::WHILE, "w");
    Synonym* synonymIf = new Synonym(Synonym::DesignEntity::IF, "ifs");
    Synonym* synonymVariable = new Synonym(Synonym::DesignEntity::VARIABLE, "v");
    Synonym* synonymConstant = new Synonym(Synonym::DesignEntity::CONSTANT, "c");

    StatementNumber* statementNumber = new StatementNumber(1);
    Wildcard* wildcard = new Wildcard();

    Clause *clause;
    Follows *follows;
    SuchThatClauseParser stcp;
    std::unique_ptr<ILexer> lexer;

    query = "such that Follows(s,p)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator1(lexer);
    requireThrow([&tokenValidator1, &stcp, &declarationList]() {
        stcp.parse(tokenValidator1, declarationList);
    });

    query = "such that Follows(s,s1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);
    clause = stcp.parse(tokenValidator, declarationList);
    follows = new Follows(synonymStatement, synonymStatement1, false);
    requireTrue(*clause == *follows);

    query = "such that Follows(s,r)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator2(lexer);
    clause = stcp.parse(tokenValidator2, declarationList);
    follows = new Follows(synonymStatement, synonymRead, false);
    requireTrue(*clause == *follows);

    query = "such that Follows(s,pn)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator3(lexer);
    clause = stcp.parse(tokenValidator3, declarationList);
    follows = new Follows(synonymStatement, synonymPrint, false);
    requireTrue(*clause == *follows);

    query = "such that Follows(s,a)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator4(lexer);
    clause = stcp.parse(tokenValidator4, declarationList);
    follows = new Follows(synonymStatement, synonymAssign, false);
    requireTrue(*clause == *follows);

    query = "such that Follows(s,cl)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator5(lexer);
    clause = stcp.parse(tokenValidator5, declarationList);
    follows = new Follows(synonymStatement, synonymCall, false);
    requireTrue(*clause == *follows);

    query = "such that Follows(s,w)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator6(lexer);
    clause = stcp.parse(tokenValidator6, declarationList);
    follows = new Follows(synonymStatement, synonymWhile, false);
    requireTrue(*clause == *follows);

    query = "such that Follows(s,ifs)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator7(lexer);
    clause = stcp.parse(tokenValidator7, declarationList);
    follows = new Follows(synonymStatement, synonymIf, false);
    requireTrue(*clause == *follows);

    query = "such that Follows(s,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator8(lexer);
    clause = stcp.parse(tokenValidator8, declarationList);
    follows = new Follows(synonymStatement, wildcard, false);
    requireTrue(*clause == *follows);

    query = "such that Follows(s,1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator9(lexer);
    clause = stcp.parse(tokenValidator9, declarationList);
    follows = new Follows(synonymStatement, statementNumber, false);
    requireTrue(*clause == *follows);

    query = "such that Follows(_,s)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator10(lexer);
    clause = stcp.parse(tokenValidator10, declarationList);
    follows = new Follows(wildcard, synonymStatement, false);
    requireTrue(*clause == *follows);

    query = "such that Follows(_,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator11(lexer);
    clause = stcp.parse(tokenValidator11, declarationList);
    follows = new Follows(wildcard, wildcard, false);
    requireTrue(*clause == *follows);

    query = "such that Follows(_,1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator12(lexer);
    clause = stcp.parse(tokenValidator12, declarationList);
    follows = new Follows(wildcard, statementNumber, false);
    requireTrue(*clause == *follows);

    query = "such that Follows(_,p)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator13(lexer);
    requireThrow([&tokenValidator13, &stcp, &declarationList]() {
        stcp.parse(tokenValidator13, declarationList);
    });

    query = "such that Follows(1,pn)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator14(lexer);
    clause = stcp.parse(tokenValidator14, declarationList);
    follows = new Follows(statementNumber, synonymPrint, false);
    requireTrue(*clause == *follows);

    query = "such that Follows(1,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator15(lexer);
    clause = stcp.parse(tokenValidator15, declarationList);
    follows = new Follows(statementNumber, wildcard, false);
    requireTrue(*clause == *follows);

    query = "such that Follows(1,1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator16(lexer);
    clause = stcp.parse(tokenValidator16, declarationList);
    follows = new Follows(statementNumber, statementNumber, false);
    requireTrue(*clause == *follows);
}


TEST_CASE("SuchThatClauseParser parse(); Parent, argument type test") {
    std::string query = "";
    std::vector<Synonym> declarationList;
    declarationList.push_back(Synonym(Synonym::DesignEntity::PROCEDURE, "p"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::STMT, "s"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::STMT, "s1"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::READ, "r"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::PRINT, "pn"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::ASSIGN, "a"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::CALL, "cl"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::WHILE, "w"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::IF, "ifs"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::VARIABLE, "v"));
    declarationList.push_back(Synonym(Synonym::DesignEntity::CONSTANT, "c"));

    Synonym* synonymProcedure = new Synonym(Synonym::DesignEntity::PROCEDURE, "p");
    Synonym* synonymStatement = new Synonym(Synonym::DesignEntity::STMT, "s");
    Synonym* synonymStatement1 = new Synonym(Synonym::DesignEntity::STMT, "s1");
    Synonym* synonymRead = new Synonym(Synonym::DesignEntity::READ, "r");
    Synonym* synonymPrint = new Synonym(Synonym::DesignEntity::PRINT, "pn");
    Synonym* synonymAssign = new Synonym(Synonym::DesignEntity::ASSIGN, "a");
    Synonym* synonymCall = new Synonym(Synonym::DesignEntity::CALL, "cl");
    Synonym* synonymWhile = new Synonym(Synonym::DesignEntity::WHILE, "w");
    Synonym* synonymIf = new Synonym(Synonym::DesignEntity::IF, "ifs");
    Synonym* synonymVariable = new Synonym(Synonym::DesignEntity::VARIABLE, "v");
    Synonym* synonymConstant = new Synonym(Synonym::DesignEntity::CONSTANT, "c");

    StatementNumber* statementNumber = new StatementNumber(1);
    Wildcard* wildcard = new Wildcard();

    Clause *clause;
    Parent *parent;
    SuchThatClauseParser stcp;
    std::unique_ptr<ILexer> lexer;

    query = "such that Follows(s,p)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator1(lexer);
    requireThrow([&tokenValidator1, &stcp, &declarationList]() {
        stcp.parse(tokenValidator1, declarationList);
    });

    query = "such that Parent(s,s1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);
    clause = stcp.parse(tokenValidator, declarationList);
    parent = new Parent(synonymStatement, synonymStatement1, false);
    requireTrue(*clause == *parent);

    query = "such that Parent(s,r)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator2(lexer);
    clause = stcp.parse(tokenValidator2, declarationList);
    parent = new Parent(synonymStatement, synonymRead, false);
    requireTrue(*clause == *parent);

    query = "such that Parent(s,pn)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator3(lexer);
    clause = stcp.parse(tokenValidator3, declarationList);
    parent = new Parent(synonymStatement, synonymPrint, false);
    requireTrue(*clause == *parent);

    query = "such that Parent(s,a)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator4(lexer);
    clause = stcp.parse(tokenValidator4, declarationList);
    parent = new Parent(synonymStatement, synonymAssign, false);
    requireTrue(*clause == *parent);

    query = "such that Parent(s,cl)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator5(lexer);
    clause = stcp.parse(tokenValidator5, declarationList);
    parent = new Parent(synonymStatement, synonymCall, false);
    requireTrue(*clause == *parent);

    query = "such that Parent(s,w)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator6(lexer);
    clause = stcp.parse(tokenValidator6, declarationList);
    parent = new Parent(synonymStatement, synonymWhile, false);
    requireTrue(*clause == *parent);

    query = "such that Parent(s,ifs)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator7(lexer);
    clause = stcp.parse(tokenValidator7, declarationList);
    parent = new Parent(synonymStatement, synonymIf, false);
    requireTrue(*clause == *parent);

    query = "such that Parent(s,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator8(lexer);
    clause = stcp.parse(tokenValidator8, declarationList);
    parent = new Parent(synonymStatement, wildcard, false);
    requireTrue(*clause == *parent);

    query = "such that Parent(s,1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator9(lexer);
    clause = stcp.parse(tokenValidator9, declarationList);
    parent = new Parent(synonymStatement, statementNumber, false);
    requireTrue(*clause == *parent);

    query = "such that Parent(_,s)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator10(lexer);
    clause = stcp.parse(tokenValidator10, declarationList);
    parent = new Parent(wildcard, synonymStatement, false);
    requireTrue(*clause == *parent);

    query = "such that Parent(_,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator11(lexer);
    clause = stcp.parse(tokenValidator11, declarationList);
    parent = new Parent(wildcard, wildcard, false);
    requireTrue(*clause == *parent);

    query = "such that Parent(_,1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator12(lexer);
    clause = stcp.parse(tokenValidator12, declarationList);
    parent = new Parent(wildcard, statementNumber, false);
    requireTrue(*clause == *parent);

    query = "such that Parent(_,p)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator13(lexer);
    requireThrow([&tokenValidator13, &stcp, &declarationList]() {
        stcp.parse(tokenValidator13, declarationList);
    });

    query = "such that Parent(1,pn)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator14(lexer);
    clause = stcp.parse(tokenValidator14, declarationList);
    parent = new Parent(statementNumber, synonymPrint, false);
    requireTrue(*clause == *parent);

    query = "such that Parent(1,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator15(lexer);
    clause = stcp.parse(tokenValidator15, declarationList);
    parent = new Parent(statementNumber, wildcard, false);
    requireTrue(*clause == *parent);

    query = "such that Parent(1,1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator16(lexer);
    clause = stcp.parse(tokenValidator16, declarationList);
    parent = new Parent(statementNumber, statementNumber, false);
    requireTrue(*clause == *parent);
}
