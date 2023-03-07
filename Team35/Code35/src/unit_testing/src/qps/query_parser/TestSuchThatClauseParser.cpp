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

class setUpStcp {
public:
    setUpStcp() {
        // Set up code
        query = "";
        declarationList.emplace_back(Synonym::DesignEntity::PROCEDURE, "p");
        declarationList.emplace_back(Synonym::DesignEntity::STMT, "s");
        declarationList.emplace_back(Synonym::DesignEntity::STMT, "s1");
        declarationList.emplace_back(Synonym::DesignEntity::READ, "r");
        declarationList.emplace_back(Synonym::DesignEntity::PRINT, "pn");
        declarationList.emplace_back(Synonym::DesignEntity::ASSIGN, "a");
        declarationList.emplace_back(Synonym::DesignEntity::CALL, "cl");
        declarationList.emplace_back(Synonym::DesignEntity::WHILE, "w");
        declarationList.emplace_back(Synonym::DesignEntity::IF, "ifs");
        declarationList.emplace_back(Synonym::DesignEntity::VARIABLE, "v");
        declarationList.emplace_back(Synonym::DesignEntity::CONSTANT, "c");

        synonymProcedure = std::make_unique<Synonym>(Synonym::DesignEntity::PROCEDURE, "p");
        synonymStatement = std::make_unique<Synonym>(Synonym::DesignEntity::STMT, "s");
        synonymStatement1 = std::make_unique<Synonym>(Synonym::DesignEntity::STMT, "s1");
        synonymRead = std::make_unique<Synonym>(Synonym::DesignEntity::READ, "r");
        synonymPrint = std::make_unique<Synonym>(Synonym::DesignEntity::PRINT, "pn");
        synonymAssign = std::make_unique<Synonym>(Synonym::DesignEntity::ASSIGN, "a");
        synonymCall = std::make_unique<Synonym>(Synonym::DesignEntity::CALL, "cl");
        synonymWhile = std::make_unique<Synonym>(Synonym::DesignEntity::WHILE, "w");
        synonymIf = std::make_unique<Synonym>(Synonym::DesignEntity::IF, "ifs");
        synonymVariable = std::make_unique<Synonym>(Synonym::DesignEntity::VARIABLE, "v");
        synonymConstant = std::make_unique<Synonym>(Synonym::DesignEntity::CONSTANT, "c");

        identStr = std::make_unique<Ident>("x");
        statementNumber1 = std::make_unique<StatementNumber>(1);
        statementNumber2 = std::make_unique<StatementNumber>(1);
        wildcard1 = std::make_unique<Wildcard>();
        wildcard2 = std::make_unique<Wildcard>();
    }

    ~setUpStcp() {
        // Tear down code
    }

    // Declarations
    std::string query;
    std::vector<Synonym> declarationList;

    std::unique_ptr<Synonym> synonymProcedure;
    std::unique_ptr<Synonym> synonymStatement;
    std::unique_ptr<Synonym> synonymStatement1;
    std::unique_ptr<Synonym> synonymRead;
    std::unique_ptr<Synonym> synonymPrint;
    std::unique_ptr<Synonym> synonymAssign;
    std::unique_ptr<Synonym> synonymCall;
    std::unique_ptr<Synonym> synonymWhile;
    std::unique_ptr<Synonym> synonymIf;
    std::unique_ptr<Synonym> synonymVariable;
    std::unique_ptr<Synonym> synonymConstant;

    std::unique_ptr<Ident> identStr;
    std::unique_ptr<StatementNumber> statementNumber1;
    std::unique_ptr<StatementNumber> statementNumber2;
    std::unique_ptr<Wildcard> wildcard1;
    std::unique_ptr<Wildcard> wildcard2;

    std::unique_ptr<Clause> clause;
    std::unique_ptr<ModifiesS> modifiesS;
    std::unique_ptr<UsesS> usesS;
    std::unique_ptr<Follows> follows;
    std::unique_ptr<Parent> parent;
    std::unique_ptr<SuchThatClauseParser> stcp;
    std::unique_ptr<ILexer> lexer;
};

TEST_CASE_METHOD(setUpStcp, "Modifies, statement and variable") {
    query = "such that Modifies(s,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator2(lexer);
    clause = stcp->parse(tokenValidator2, declarationList);
    modifiesS = std::make_unique<ModifiesS>(std::move(synonymStatement), std::move(synonymVariable));
    requireTrue(*clause == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Modifies, read and variable") {
    query = "such that Modifies(r,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator3(lexer);
    clause = stcp->parse(tokenValidator3, declarationList);
    modifiesS = std::make_unique<ModifiesS>(std::move(synonymRead), std::move(synonymVariable));
    requireTrue(*clause == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Modifies, assign and variable") {
    query = "such that Modifies(a,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator5(lexer);
    clause = stcp->parse(tokenValidator5, declarationList);
    modifiesS = std::make_unique<ModifiesS>(std::move(synonymAssign), std::move(synonymVariable));
    requireTrue(*clause == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Modifies, call and variable") {
    query = "such that Modifies(cl,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator6(lexer);
    clause = stcp->parse(tokenValidator6, declarationList);
    modifiesS = std::make_unique<ModifiesS>(std::move(synonymCall), std::move(synonymVariable));
    requireTrue(*clause == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Modifies, while and variable") {
    query = "such that Modifies(w,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator6(lexer);
    clause = stcp->parse(tokenValidator6, declarationList);
    modifiesS = std::make_unique<ModifiesS>(std::move(synonymWhile), std::move(synonymVariable));
    requireTrue(*clause == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Modifies, if and variable") {
    query = "such that Modifies(ifs,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator8(lexer);
    clause = stcp->parse(tokenValidator8, declarationList);
    modifiesS = std::make_unique<ModifiesS>(std::move(synonymIf), std::move(synonymVariable));
    requireTrue(*clause == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Modifies, statement and ident") {
    query = "such that Modifies(s,\"x\")";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator9(lexer);
    clause = stcp->parse(tokenValidator9, declarationList);
    modifiesS = std::make_unique<ModifiesS>(std::move(synonymStatement), std::move(identStr));
    requireTrue(*clause == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Modifies, statement and wildcard") {
    query = "such that Modifies(s,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator10(lexer);
    clause = stcp->parse(tokenValidator10, declarationList);
    modifiesS = std::make_unique<ModifiesS>(std::move(synonymStatement), std::move(wildcard1));
    requireTrue(*clause == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Modifies, int and variable") {
    query = "such that Modifies(1,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator11(lexer);
    clause = stcp->parse(tokenValidator11, declarationList);
    modifiesS = std::make_unique<ModifiesS>(std::move(statementNumber1), std::move(synonymVariable));
    requireTrue(*clause == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Modifies, int and ident") {
    query = "such that Modifies(1,\"x\")";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator12(lexer);
    clause = stcp->parse(tokenValidator12, declarationList);
    modifiesS = std::make_unique<ModifiesS>(std::move(statementNumber1), std::move(identStr));
    requireTrue(*clause == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Modifies, int and wildcard") {
    query = "such that Modifies(1,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator13(lexer);
    clause = stcp->parse(tokenValidator13, declarationList);
    modifiesS = std::make_unique<ModifiesS>(std::move(statementNumber1), std::move(wildcard1));
    requireTrue(*clause == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, statement and variable") {
    query = "such that Uses(s,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator2(lexer);
    clause = stcp->parse(tokenValidator2, declarationList);
    usesS = std::make_unique<UsesS>(std::move(synonymStatement), std::move(synonymVariable));
    requireTrue(*clause == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, print and variable") {
    query = "such that Uses(pn,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator3(lexer);
    clause = stcp->parse(tokenValidator3, declarationList);
    usesS = std::make_unique<UsesS>(std::move(synonymPrint), std::move(synonymVariable));
    requireTrue(*clause == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, assign and variable") {
    query = "such that Uses(a,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator5(lexer);
    clause = stcp->parse(tokenValidator5, declarationList);
    usesS = std::make_unique<UsesS>(std::move(synonymAssign), std::move(synonymVariable));
    requireTrue(*clause == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, call and variable") {
    query = "such that Uses(cl,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator6(lexer);
    clause = stcp->parse(tokenValidator6, declarationList);
    usesS = std::make_unique<UsesS>(std::move(synonymCall), std::move(synonymVariable));
    requireTrue(*clause == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, while and variable") {
    query = "such that Uses(w,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator7(lexer);
    clause = stcp->parse(tokenValidator7, declarationList);
    usesS = std::make_unique<UsesS>(std::move(synonymWhile), std::move(synonymVariable));
    requireTrue(*clause == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, if and variable") {
    query = "such that Uses(ifs,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator8(lexer);
    clause = stcp->parse(tokenValidator8, declarationList);
    usesS = std::make_unique<UsesS>(std::move(synonymIf), std::move(synonymVariable));
    requireTrue(*clause == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, statement and ident") {
    query = "such that Uses(s,\"x\")";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator9(lexer);
    clause = stcp->parse(tokenValidator9, declarationList);
    usesS = std::make_unique<UsesS>(std::move(synonymStatement), std::move(identStr));
    requireTrue(*clause == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, statement and wildcard") {
    query = "such that Uses(s,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator10(lexer);
    clause = stcp->parse(tokenValidator10, declarationList);
    usesS = std::make_unique<UsesS>(std::move(synonymStatement), std::move(wildcard1));
    requireTrue(*clause == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, int and variable") {
    query = "such that Uses(1,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator11(lexer);
    clause = stcp->parse(tokenValidator11, declarationList);
    usesS = std::make_unique<UsesS>(std::move(statementNumber1), std::move(synonymVariable));
    requireTrue(*clause == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, int and ident") {
    query = "such that Uses(1,\"x\")";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator12(lexer);
    clause = stcp->parse(tokenValidator12, declarationList);
    usesS = std::make_unique<UsesS>(std::move(statementNumber1), std::move(identStr));
    requireTrue(*clause == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, int and wildcard") {
    query = "such that Uses(1,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator13(lexer);
    clause = stcp->parse(tokenValidator13, declarationList);
    usesS = std::make_unique<UsesS>(std::move(statementNumber1), std::move(wildcard1));
    requireTrue(*clause == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Follows, statement and statement") {
    query = "such that Follows(s,s1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);
    clause = stcp->parse(tokenValidator, declarationList);
    follows = std::make_unique<Follows>(std::move(synonymStatement), std::move(synonymStatement1), false);
    requireTrue(*clause == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, statement and read") {
    query = "such that Follows(s,r)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator2(lexer);
    clause = stcp->parse(tokenValidator2, declarationList);
    follows = std::make_unique<Follows>(std::move(synonymStatement), std::move(synonymRead), false);
    requireTrue(*clause == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, statement and print") {
    query = "such that Follows(s,pn)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator3(lexer);
    clause = stcp->parse(tokenValidator3, declarationList);
    follows = std::make_unique<Follows>(std::move(synonymStatement), std::move(synonymPrint), false);
    requireTrue(*clause == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, statement and assign") {
    query = "such that Follows(s,a)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator4(lexer);
    clause = stcp->parse(tokenValidator4, declarationList);
    follows = std::make_unique<Follows>(std::move(synonymStatement), std::move(synonymAssign), false);
    requireTrue(*clause == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, statement and call") {
    query = "such that Follows(s,cl)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator5(lexer);
    clause = stcp->parse(tokenValidator5, declarationList);
    follows = std::make_unique<Follows>(std::move(synonymStatement), std::move(synonymCall), false);
    requireTrue(*clause == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, statement and while") {
    query = "such that Follows(s,w)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator6(lexer);
    clause = stcp->parse(tokenValidator6, declarationList);
    follows = std::make_unique<Follows>(std::move(synonymStatement), std::move(synonymWhile), false);
    requireTrue(*clause == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, statement and if") {
    query = "such that Follows(s,ifs)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator7(lexer);
    clause = stcp->parse(tokenValidator7, declarationList);
    follows = std::make_unique<Follows>(std::move(synonymStatement), std::move(synonymIf), false);
    requireTrue(*clause == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, statement and wildcard") {
    query = "such that Follows(s,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator8(lexer);
    clause = stcp->parse(tokenValidator8, declarationList);
    follows = std::make_unique<Follows>(std::move(synonymStatement), std::move(wildcard1), false);
    requireTrue(*clause == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, statement and int") {
    query = "such that Follows(s,1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator9(lexer);
    clause = stcp->parse(tokenValidator9, declarationList);
    follows = std::make_unique<Follows>(std::move(synonymStatement), std::move(statementNumber1), false);
    requireTrue(*clause == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, wildcard and statement") {
    query = "such that Follows(_,s)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator10(lexer);
    clause = stcp->parse(tokenValidator10, declarationList);
    follows = std::make_unique<Follows>(std::move(wildcard1), std::move(synonymStatement), false);
    requireTrue(*clause == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, wildcard and wildcard") {
    query = "such that Follows(_,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator11(lexer);
    clause = stcp->parse(tokenValidator11, declarationList);
    follows = std::make_unique<Follows>(std::move(wildcard1), std::move(wildcard2), false);
    requireTrue(*clause == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, wildcard and int") {
    query = "such that Follows(_,1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator12(lexer);
    clause = stcp->parse(tokenValidator12, declarationList);
    follows = std::make_unique<Follows>(std::move(wildcard1), std::move(statementNumber1), false);
    requireTrue(*clause == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, int and wildcard") {
    query = "such that Follows(1,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator15(lexer);
    clause = stcp->parse(tokenValidator15, declarationList);
    follows = std::make_unique<Follows>(std::move(statementNumber1), std::move(wildcard1), false);
    requireTrue(*clause == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, int and int") {
    query = "such that Follows(1,1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator16(lexer);
    clause = stcp->parse(tokenValidator16, declarationList);
    follows = std::make_unique<Follows>(std::move(statementNumber1), std::move(statementNumber2), false);
    requireTrue(*clause == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Parent, statement and statement") {
    query = "such that Parent(s,s1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);
    clause = stcp->parse(tokenValidator, declarationList);
    parent = std::make_unique<Parent>(std::move(synonymStatement), std::move(synonymStatement1), false);
    requireTrue(*clause == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, statement and read") {
    query = "such that Parent(s,r)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator2(lexer);
    clause = stcp->parse(tokenValidator2, declarationList);
    parent = std::make_unique<Parent>(std::move(synonymStatement), std::move(synonymRead), false);
    requireTrue(*clause == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, statement and print") {
    query = "such that Parent(s,pn)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator3(lexer);
    clause = stcp->parse(tokenValidator3, declarationList);
    parent = std::make_unique<Parent>(std::move(synonymStatement), std::move(synonymPrint), false);
    requireTrue(*clause == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, statement and assign") {
    query = "such that Parent(s,a)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator4(lexer);
    clause = stcp->parse(tokenValidator4, declarationList);
    parent = std::make_unique<Parent>(std::move(synonymStatement), std::move(synonymAssign), false);
    requireTrue(*clause == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, statement and call") {
    query = "such that Parent(s,cl)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator5(lexer);
    clause = stcp->parse(tokenValidator5, declarationList);
    parent = std::make_unique<Parent>(std::move(synonymStatement), std::move(synonymCall), false);
    requireTrue(*clause == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, statement and while") {
    query = "such that Parent(s,w)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator6(lexer);
    clause = stcp->parse(tokenValidator6, declarationList);
    parent = std::make_unique<Parent>(std::move(synonymStatement), std::move(synonymWhile), false);
    requireTrue(*clause == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, statement and if") {
    query = "such that Parent(s,ifs)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator7(lexer);
    clause = stcp->parse(tokenValidator7, declarationList);
    parent = std::make_unique<Parent>(std::move(synonymStatement), std::move(synonymIf), false);
    requireTrue(*clause == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, statement and wildcard") {
    query = "such that Parent(s,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator8(lexer);
    clause = stcp->parse(tokenValidator8, declarationList);
    parent = std::make_unique<Parent>(std::move(synonymStatement), std::move(wildcard1), false);
    requireTrue(*clause == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, statement and int") {
    query = "such that Parent(s,1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator9(lexer);
    clause = stcp->parse(tokenValidator9, declarationList);
    parent = std::make_unique<Parent>(std::move(synonymStatement), std::move(statementNumber1), false);
    requireTrue(*clause == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, wildcard and statement") {
    query = "such that Parent(_,s)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator10(lexer);
    clause = stcp->parse(tokenValidator10, declarationList);
    parent = std::make_unique<Parent>(std::move(wildcard1), std::move(synonymStatement), false);
    requireTrue(*clause == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, wildcard and wildcard") {
    query = "such that Parent(_,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator11(lexer);
    clause = stcp->parse(tokenValidator11, declarationList);
    parent = std::make_unique<Parent>(std::move(wildcard1), std::move(wildcard2), false);
    requireTrue(*clause == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, wildcard and int") {
    query = "such that Parent(_,1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator12(lexer);
    clause = stcp->parse(tokenValidator12, declarationList);
    parent = std::make_unique<Parent>(std::move(wildcard1), std::move(statementNumber1), false);
    requireTrue(*clause == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, int and print") {
    query = "such that Parent(1,pn)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator14(lexer);
    clause = stcp->parse(tokenValidator14, declarationList);
    parent = std::make_unique<Parent>(std::move(statementNumber1), std::move(synonymPrint), false);
    requireTrue(*clause == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, int and wildcard") {
    query = "such that Parent(1,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator15(lexer);
    clause = stcp->parse(tokenValidator15, declarationList);
    parent = std::make_unique<Parent>(std::move(statementNumber1), std::move(wildcard1), false);
    requireTrue(*clause == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, int and int") {
    query = "such that Parent(1,1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator16(lexer);
    clause = stcp->parse(tokenValidator16, declarationList);
    parent = std::make_unique<Parent>(std::move(statementNumber1), std::move(statementNumber2), false);
    requireTrue(*clause == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Follows*, statement and statement") {
    query = "such that Follows*(s,s1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator2(lexer);
    clause = stcp->parse(tokenValidator2, declarationList);
    follows = std::make_unique<Follows>(std::move(synonymStatement), std::move(synonymStatement1), true);
    requireTrue(*clause == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Parent*, statement and statement") {
    query = "such that Parent*(s,s1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator3(lexer);
    clause = stcp->parse(tokenValidator3, declarationList);
    parent = std::make_unique<Parent>(std::move(synonymStatement), std::move(synonymStatement1), true);
    requireTrue(*clause == *parent);
}

TEST_CASE_METHOD(setUpStcp, "invalid Modifies, statement and statement, semantic error") {
    query = "such that Modifies(s,s1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator3(lexer);
    requireThrow([&tokenValidator3]() {
        std::vector<Synonym> dl = setUpStcp().declarationList;
        setUpStcp().stcp->parse(tokenValidator3, dl);
    });
}

TEST_CASE_METHOD(setUpStcp, "invalid Follows, statement and variable, semantic error") {
    query = "such that Parent(s,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator4(lexer);
    requireThrow([&tokenValidator4]() {
        std::vector<Synonym> dl = setUpStcp().declarationList;
        setUpStcp().stcp->parse(tokenValidator4, dl);
    });
}
