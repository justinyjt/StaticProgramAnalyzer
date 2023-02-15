#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/query_parser/clause_parser/ClauseParser.h"
#include "commons/lexer/LexerFactory.h"
#include "qps/query_parser/declaration_parser/DeclarationParser.h"
#include "qps/clause/relationship/Modify.h"
#include "qps/pql/StatementNumber.h"
#include "qps/pql/IdentStr.h"
#include "qps/clause/relationship/Uses.h"

TEST_CASE("SuchThatClauseParser parse(); ModifiesS, test for semantic error") {
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

    IdentStr* identStr = new IdentStr("x");
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

TEST_CASE("SuchThatClauseParser parse(); UsesS, test for semantic error") {
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

    IdentStr* identStr = new IdentStr("x");
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
