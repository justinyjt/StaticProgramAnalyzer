#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/query_parser/clause_parser/ClauseParser.h"
#include "commons/lexer/LexerFactory.h"
#include "qps/query_parser/declaration_parser/DeclarationParser.h"
#include "qps/pql/PQLNumber.h"
#include "qps/pql/Ident.h"
#include "qps/clause/TwoArgClause/StmtEntClause.h"
#include "qps/clause/TwoArgClause/StmtStmtClause.h"
#include "qps/clause/TwoArgClause/EntEntClause.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "qps/query_parser/QuerySyntaxValidator.h"

class setUpStcp {
 public:
    setUpStcp() {
        // Set up code
        query = "";
        declarationList.insert({"p", Synonym::DesignEntity::PROCEDURE});
        declarationList.insert({"p1", Synonym::DesignEntity::PROCEDURE});
        declarationList.insert({"s", Synonym::DesignEntity::STMT});
        declarationList.insert({"s1", Synonym::DesignEntity::STMT});
        declarationList.insert({"r", Synonym::DesignEntity::READ});
        declarationList.insert({"pn", Synonym::DesignEntity::PRINT});
        declarationList.insert({"a", Synonym::DesignEntity::ASSIGN});
        declarationList.insert({"cl", Synonym::DesignEntity::CALL});
        declarationList.insert({"w", Synonym::DesignEntity::WHILE});
        declarationList.insert({"r", Synonym::DesignEntity::READ});
        declarationList.insert({"ifs", Synonym::DesignEntity::IF});
        declarationList.insert({"v", Synonym::DesignEntity::VARIABLE});
        declarationList.insert({"c", Synonym::DesignEntity::CONSTANT});

        synonymProcedure = std::make_unique<Synonym>(Synonym::DesignEntity::PROCEDURE, "p");
        synonymProcedure1 = std::make_unique<Synonym>(Synonym::DesignEntity::PROCEDURE, "p1");
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
        statementNumber1 = std::make_unique<PQLNumber>(1);
        statementNumber2 = std::make_unique<PQLNumber>(1);
        wildcard1 = std::make_unique<Wildcard>();
        wildcard2 = std::make_unique<Wildcard>();
    }

    ~setUpStcp() {
        // Tear down code
    }

    // Declarations
    std::string query;
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;

    std::unique_ptr<Synonym> synonymProcedure;
    std::unique_ptr<Synonym> synonymProcedure1;
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
    std::unique_ptr<PQLNumber> statementNumber1;
    std::unique_ptr<PQLNumber> statementNumber2;
    std::unique_ptr<Wildcard> wildcard1;
    std::unique_ptr<Wildcard> wildcard2;

    std::vector<std::unique_ptr<Clause>> clause;
    std::unique_ptr<ModifiesS> modifiesS;
    std::unique_ptr<UsesS> usesS;
    std::unique_ptr<ModifiesP> modifiesP;
    std::unique_ptr<UsesP> usesP;
    std::unique_ptr<Follows> follows;
    std::unique_ptr<Parent> parent;
    std::unique_ptr<Calls> calls;
    std::unique_ptr<ILexer> lexer;
};

TEST_CASE_METHOD(setUpStcp, "Modifies, statement and variable") {
    query = "such that Modifies(s,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    modifiesS = std::make_unique<ModifiesS>(std::move(synonymStatement), std::move(synonymVariable));
    requireTrue(*clause.front() == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Modifies, read and variable") {
    query = "such that Modifies(r,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    modifiesS = std::make_unique<ModifiesS>(std::move(synonymRead), std::move(synonymVariable));
    requireTrue(*clause.front() == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Modifies, assign and variable") {
    query = "such that Modifies(a,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    modifiesS = std::make_unique<ModifiesS>(std::move(synonymAssign), std::move(synonymVariable));
    requireTrue(*clause.front() == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Modifies, call and variable") {
    query = "such that Modifies(cl,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    modifiesS = std::make_unique<ModifiesS>(std::move(synonymCall), std::move(synonymVariable));
    requireTrue(*clause.front() == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Modifies, while and variable") {
    query = "such that Modifies(w,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    modifiesS = std::make_unique<ModifiesS>(std::move(synonymWhile), std::move(synonymVariable));
    requireTrue(*clause.front() == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Modifies, if and variable") {
    query = "such that Modifies(ifs,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    modifiesS = std::make_unique<ModifiesS>(std::move(synonymIf), std::move(synonymVariable));
    requireTrue(*clause.front() == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Modifies, statement and ident") {
    query = "such that Modifies(s,\"x\")";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    modifiesS = std::make_unique<ModifiesS>(std::move(synonymStatement), std::move(identStr));
    requireTrue(*clause.front() == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Modifies, statement and wildcard") {
    query = "such that Modifies(s,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    modifiesS = std::make_unique<ModifiesS>(std::move(synonymStatement), std::move(wildcard1));
    requireTrue(*clause.front() == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Modifies, int and variable") {
    query = "such that Modifies(1,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    modifiesS = std::make_unique<ModifiesS>(std::move(statementNumber1), std::move(synonymVariable));
    requireTrue(*clause.front() == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Modifies, int and ident") {
    query = "such that Modifies(1,\"x\")";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    modifiesS = std::make_unique<ModifiesS>(std::move(statementNumber1), std::move(identStr));
    requireTrue(*clause.front() == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Modifies, int and wildcard") {
    query = "such that Modifies(1,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    modifiesS = std::make_unique<ModifiesS>(std::move(statementNumber1), std::move(wildcard1));
    requireTrue(*clause.front() == *modifiesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, statement and variable") {
    query = "such that Uses(s,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    usesS = std::make_unique<UsesS>(std::move(synonymStatement), std::move(synonymVariable));
    requireTrue(*clause.front() == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, print and variable") {
    query = "such that Uses(pn,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    usesS = std::make_unique<UsesS>(std::move(synonymPrint), std::move(synonymVariable));
    requireTrue(*clause.front() == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, assign and variable") {
    query = "such that Uses(a,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    usesS = std::make_unique<UsesS>(std::move(synonymAssign), std::move(synonymVariable));
    requireTrue(*clause.front() == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, call and variable") {
    query = "such that Uses(cl,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    usesS = std::make_unique<UsesS>(std::move(synonymCall), std::move(synonymVariable));
    requireTrue(*clause.front() == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, while and variable") {
    query = "such that Uses(w,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    usesS = std::make_unique<UsesS>(std::move(synonymWhile), std::move(synonymVariable));
    requireTrue(*clause.front() == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, if and variable") {
    query = "such that Uses(ifs,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    usesS = std::make_unique<UsesS>(std::move(synonymIf), std::move(synonymVariable));
    requireTrue(*clause.front() == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, statement and ident") {
    query = "such that Uses(s,\"x\")";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    usesS = std::make_unique<UsesS>(std::move(synonymStatement), std::move(identStr));
    requireTrue(*clause.front() == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, statement and wildcard") {
    query = "such that Uses(s,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    usesS = std::make_unique<UsesS>(std::move(synonymStatement), std::move(wildcard1));
    requireTrue(*clause.front() == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, int and variable") {
    query = "such that Uses(1,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    usesS = std::make_unique<UsesS>(std::move(statementNumber1), std::move(synonymVariable));
    requireTrue(*clause.front() == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, int and ident") {
    query = "such that Uses(1,\"x\")";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    usesS = std::make_unique<UsesS>(std::move(statementNumber1), std::move(identStr));
    requireTrue(*clause.front() == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Uses, int and wildcard") {
    query = "such that Uses(1,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    usesS = std::make_unique<UsesS>(std::move(statementNumber1), std::move(wildcard1));
    requireTrue(*clause.front() == *usesS);
}

TEST_CASE_METHOD(setUpStcp, "Follows, statement and statement") {
    query = "such that Follows(s,s1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    follows = std::make_unique<Follows>(std::move(synonymStatement), std::move(synonymStatement1), false);
    requireTrue(*clause.front() == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, statement and read") {
    query = "such that Follows(s,r)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    follows = std::make_unique<Follows>(std::move(synonymStatement), std::move(synonymRead), false);
    requireTrue(*clause.front() == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, statement and print") {
    query = "such that Follows(s,pn)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    follows = std::make_unique<Follows>(std::move(synonymStatement), std::move(synonymPrint), false);
    requireTrue(*clause.front() == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, statement and assign") {
    query = "such that Follows(s,a)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    follows = std::make_unique<Follows>(std::move(synonymStatement), std::move(synonymAssign), false);
    requireTrue(*clause.front() == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, statement and call") {
    query = "such that Follows(s,cl)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    follows = std::make_unique<Follows>(std::move(synonymStatement), std::move(synonymCall), false);
    requireTrue(*clause.front() == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, statement and while") {
    query = "such that Follows(s,w)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    follows = std::make_unique<Follows>(std::move(synonymStatement), std::move(synonymWhile), false);
    requireTrue(*clause.front() == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, statement and if") {
    query = "such that Follows(s,ifs)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    follows = std::make_unique<Follows>(std::move(synonymStatement), std::move(synonymIf), false);
    requireTrue(*clause.front() == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, statement and wildcard") {
    query = "such that Follows(s,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    follows = std::make_unique<Follows>(std::move(synonymStatement), std::move(wildcard1), false);
    requireTrue(*clause.front() == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, statement and int") {
    query = "such that Follows(s,1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    follows = std::make_unique<Follows>(std::move(synonymStatement), std::move(statementNumber1), false);
    requireTrue(*clause.front() == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, wildcard and statement") {
    query = "such that Follows(_,s)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    follows = std::make_unique<Follows>(std::move(wildcard1), std::move(synonymStatement), false);
    requireTrue(*clause.front() == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, wildcard and wildcard") {
    query = "such that Follows(_,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    follows = std::make_unique<Follows>(std::move(wildcard1), std::move(wildcard2), false);
    requireTrue(*clause.front() == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, wildcard and int") {
    query = "such that Follows(_,1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    follows = std::make_unique<Follows>(std::move(wildcard1), std::move(statementNumber1), false);
    requireTrue(*clause.front() == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, int and wildcard") {
    query = "such that Follows(1,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    follows = std::make_unique<Follows>(std::move(statementNumber1), std::move(wildcard1), false);
    requireTrue(*clause.front() == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Follows, int and int") {
    query = "such that Follows(1,1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    follows = std::make_unique<Follows>(std::move(statementNumber1), std::move(statementNumber2), false);
    requireTrue(*clause.front() == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Parent, statement and statement") {
    query = "such that Parent(s,s1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    parent = std::make_unique<Parent>(std::move(synonymStatement), std::move(synonymStatement1), false);
    requireTrue(*clause.front() == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, statement and read") {
    query = "such that Parent(s,r)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    parent = std::make_unique<Parent>(std::move(synonymStatement), std::move(synonymRead), false);
    requireTrue(*clause.front() == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, statement and print") {
    query = "such that Parent(s,pn)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    parent = std::make_unique<Parent>(std::move(synonymStatement), std::move(synonymPrint), false);
    requireTrue(*clause.front() == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, statement and assign") {
    query = "such that Parent(s,a)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    parent = std::make_unique<Parent>(std::move(synonymStatement), std::move(synonymAssign), false);
    requireTrue(*clause.front() == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, statement and call") {
    query = "such that Parent(s,cl)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    parent = std::make_unique<Parent>(std::move(synonymStatement), std::move(synonymCall), false);
    requireTrue(*clause.front() == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, statement and while") {
    query = "such that Parent(s,w)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    parent = std::make_unique<Parent>(std::move(synonymStatement), std::move(synonymWhile), false);
    requireTrue(*clause.front() == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, statement and if") {
    query = "such that Parent(s,ifs)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    parent = std::make_unique<Parent>(std::move(synonymStatement), std::move(synonymIf), false);
    requireTrue(*clause.front() == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, statement and wildcard") {
    query = "such that Parent(s,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    parent = std::make_unique<Parent>(std::move(synonymStatement), std::move(wildcard1), false);
    requireTrue(*clause.front() == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, statement and int") {
    query = "such that Parent(s,1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    parent = std::make_unique<Parent>(std::move(synonymStatement), std::move(statementNumber1), false);
    requireTrue(*clause.front() == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, wildcard and statement") {
    query = "such that Parent(_,s)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    parent = std::make_unique<Parent>(std::move(wildcard1), std::move(synonymStatement), false);
    requireTrue(*clause.front() == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, wildcard and wildcard") {
    query = "such that Parent(_,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    parent = std::make_unique<Parent>(std::move(wildcard1), std::move(wildcard2), false);
    requireTrue(*clause.front() == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, wildcard and int") {
    query = "such that Parent(_,1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    parent = std::make_unique<Parent>(std::move(wildcard1), std::move(statementNumber1), false);
    requireTrue(*clause.front() == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, int and print") {
    query = "such that Parent(1,pn)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    parent = std::make_unique<Parent>(std::move(statementNumber1), std::move(synonymPrint), false);
    requireTrue(*clause.front() == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, int and wildcard") {
    query = "such that Parent(1,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    parent = std::make_unique<Parent>(std::move(statementNumber1), std::move(wildcard1), false);
    requireTrue(*clause.front() == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Parent, int and int") {
    query = "such that Parent(1,1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    parent = std::make_unique<Parent>(std::move(statementNumber1), std::move(statementNumber2), false);
    requireTrue(*clause.front() == *parent);
}

TEST_CASE_METHOD(setUpStcp, "Follows*, statement and statement") {
    query = "such that Follows*(s,s1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    follows = std::make_unique<Follows>(std::move(synonymStatement), std::move(synonymStatement1), true);
    requireTrue(*clause.front() == *follows);
}

TEST_CASE_METHOD(setUpStcp, "Parent*, statement and statement") {
    query = "such that Parent*(s,s1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    parent = std::make_unique<Parent>(std::move(synonymStatement), std::move(synonymStatement1), true);
    requireTrue(*clause.front() == *parent);
}

TEST_CASE_METHOD(setUpStcp, "invalid Modifies, statement and statement, semantic error") {
    query = "such that Modifies(s,s1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&stcp]() {
        stcp.parse();
    });
}

TEST_CASE_METHOD(setUpStcp, "invalid Modifies, wildcard and statement, semantic error") {
    query = "such that Modifies(_,s1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&stcp]() {
        stcp.parse();
    });
}

TEST_CASE_METHOD(setUpStcp, "invalid Follows, statement and variable, semantic error") {
    query = "such that Parent(s,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&stcp]() {
        stcp.parse();
    });
}

TEST_CASE_METHOD(setUpStcp, "invalid Follows, variable and statement, semantic error") {
    query = "such that Parent(v,s)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&stcp]() {
        stcp.parse();
    });
}

TEST_CASE_METHOD(setUpStcp, "Calls, procedure and procedure") {
    query = "such that Calls(p,p1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    calls = std::make_unique<Calls>(std::move(synonymProcedure), std::move(synonymProcedure1), false);
    requireTrue(*clause.front() == *calls);
}

TEST_CASE_METHOD(setUpStcp, "Calls*, procedure and procedure") {
    query = "such that Calls*(p,p1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    calls = std::make_unique<Calls>(std::move(synonymProcedure), std::move(synonymProcedure1), true);
    requireTrue(*clause.front() == *calls);
}

TEST_CASE_METHOD(setUpStcp, "Calls, int and procedure, syntax error") {
    query = "such that Calls(1,p1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(std::move(lexer));
    requireTrue(!sv->validateQuery());
}

TEST_CASE_METHOD(setUpStcp, "Calls, procedure and int, syntax error") {
    query = "such that Calls(p1,1)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(std::move(lexer));
    requireTrue(!sv->validateQuery());
}

TEST_CASE_METHOD(setUpStcp, "Calls, stmt and procedure, semantic error") {
    query = "such that Calls(s,p)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&stcp]() {
        stcp.parse();
    });
}

TEST_CASE_METHOD(setUpStcp, "Calls, ident and procedure, semantic error") {
    query = "such that Calls(\"x\",p)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SuchThatClauseParser stcp(pqlTokenScanner, declarationList);
    clause = stcp.parse();
    calls = std::make_unique<Calls>(std::move(identStr), std::move(synonymProcedure), false);
    requireTrue(*clause.front() == *calls);
}

TEST_CASE_METHOD(setUpStcp, "Calls, invalid syntax ident and procedure, syntax error") {
    query = "such that Calls(\"+x\",p)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(std::move(lexer));
    requireTrue(!sv->validateQuery());
}
