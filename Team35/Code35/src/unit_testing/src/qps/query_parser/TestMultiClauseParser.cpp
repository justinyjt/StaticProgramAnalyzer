#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/pql/Synonym.h"
#include "qps/clause/Clause.h"
#include "qps/query_parser/clause_parser/pattern_clause_parser/PatternClauseParser.h"
#include "commons/lexer/LexerFactory.h"
#include "qps/pql/Ident.h"
#include "qps/pql/Expression.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "qps/clause/one_arg_clause/WhilePattern.h"
#include "qps/clause/one_arg_clause/IfPattern.h"
#include "qps/clause/TwoArgClause/StmtEntClause.h"
#include "qps/clause/TwoArgClause/EntEntClause.h"
#include "qps/clause/TwoArgClause/StmtStmtClause.h"
#include "qps/clause/TwoArgClause/WithEntClause.h"
#include "qps/clause/TwoArgClause/WithNumClause.h"
#include "qps/query_parser/clause_parser/ClauseParser.h"
#include "qps/query_parser/QuerySyntaxValidator.h"

class setUpMcp {
public:
    setUpMcp() {
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
        synonymStatement1 = std::make_unique<Synonym>(Synonym::DesignEntity::STMT, "s");
        synonymStatement2 = std::make_unique<Synonym>(Synonym::DesignEntity::STMT, "s");
        synonymRead = std::make_unique<Synonym>(Synonym::DesignEntity::READ, "r");
        synonymPrint1 = std::make_unique<Synonym>(Synonym::DesignEntity::PRINT, "pn");
        synonymPrint2 = std::make_unique<Synonym>(Synonym::DesignEntity::PRINT, "pn");
        synonymAssign = std::make_unique<Synonym>(Synonym::DesignEntity::ASSIGN, "a");
        synonymCall = std::make_unique<Synonym>(Synonym::DesignEntity::CALL, "cl");
        synonymWhile = std::make_unique<Synonym>(Synonym::DesignEntity::WHILE, "w");
        synonymIf = std::make_unique<Synonym>(Synonym::DesignEntity::IF, "ifs");
        synonymVariable1 = std::make_unique<Synonym>(Synonym::DesignEntity::VARIABLE, "v");
        synonymVariable2 = std::make_unique<Synonym>(Synonym::DesignEntity::VARIABLE, "v");
        synonymConstant = std::make_unique<Synonym>(Synonym::DesignEntity::CONSTANT, "c");

        identStr = std::make_unique<Ident>("x");
        statementNumber1 = std::make_unique<StatementNumber>(1);
        statementNumber2 = std::make_unique<StatementNumber>(1);
        wildcard1 = std::make_unique<Wildcard>();
        wildcard2 = std::make_unique<Wildcard>();
    }

    ~setUpMcp() {
        // Tear down code
    }

    // Declarations
    std::string query;
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;

    std::unique_ptr<Synonym> synonymProcedure;
    std::unique_ptr<Synonym> synonymStatement1;
    std::unique_ptr<Synonym> synonymStatement2;
    std::unique_ptr<Synonym> synonymRead;
    std::unique_ptr<Synonym> synonymPrint1;
    std::unique_ptr<Synonym> synonymPrint2;
    std::unique_ptr<Synonym> synonymAssign;
    std::unique_ptr<Synonym> synonymCall;
    std::unique_ptr<Synonym> synonymWhile;
    std::unique_ptr<Synonym> synonymIf;
    std::unique_ptr<Synonym> synonymVariable1;
    std::unique_ptr<Synonym> synonymVariable2;
    std::unique_ptr<Synonym> synonymConstant;

    std::unique_ptr<Ident> identStr;
    std::unique_ptr<StatementNumber> statementNumber1;
    std::unique_ptr<StatementNumber> statementNumber2;
    std::unique_ptr<Wildcard> wildcard1;
    std::unique_ptr<Wildcard> wildcard2;

    std::vector<std::unique_ptr<Clause>> clause;
    std::vector<std::unique_ptr<Clause>> expected;
    std::unique_ptr<ModifiesS> modifiesS;
    std::unique_ptr<UsesS> usesS;
    std::unique_ptr<ModifiesP> modifiesP;
    std::unique_ptr<UsesP> usesP;
    std::unique_ptr<Follows> follows;
    std::unique_ptr<Parent> parent;
    std::unique_ptr<Calls> calls;
    std::unique_ptr<AssignPattern> patternAssign;
    std::unique_ptr<WhilePattern> patternWhile;
    std::unique_ptr<IfPattern> patternIf;
    std::unique_ptr<WithEntClause> withEntClause;
    std::unique_ptr<WithNumClause> withNumClause;
    std::unique_ptr<ILexer> lexer;
};

TEST_CASE_METHOD(setUpMcp, "modifiesS, usesS, invalid syntax") {
    query = "such that Modifies(s,v) Uses(s,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    ClauseParser cp(pqlTokenScanner, declarationList);
    requireThrowAs<SyntaxException>([&cp]() {
        cp.parse();
    });
}

TEST_CASE_METHOD(setUpMcp, "and start, invalid syntax") {
    query = "and Modifies(s,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    ClauseParser cp(pqlTokenScanner, declarationList);
    requireThrowAs<SyntaxException>([&cp]() {
        cp.parse();
    });
}

TEST_CASE_METHOD(setUpMcp, "such that clauses, modifiesS, modifiesS") {
    query = "such that Modifies(s,v) and Modifies(s,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    ClauseParser cp(pqlTokenScanner, declarationList);
    clause = cp.parse();
    expected.push_back(std::make_unique<ModifiesS>(std::move(synonymStatement1), std::move(synonymVariable1)));
    expected.push_back(std::make_unique<ModifiesS>(std::move(synonymStatement2), std::move(synonymVariable2)));
    requireTrue(*clause[0] == *expected[0]);
    requireTrue(*clause[1] == *expected[1]);
}

TEST_CASE_METHOD(setUpMcp, "such that clauses, modifiesS, usesS") {
    query = "such that Modifies(s,v) and Uses(s,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    ClauseParser cp(pqlTokenScanner, declarationList);
    clause = cp.parse();
    expected.push_back(std::make_unique<ModifiesS>(std::move(synonymStatement1), std::move(synonymVariable1)));
    expected.push_back(std::make_unique<UsesS>(std::move(synonymStatement2), std::move(synonymVariable2)));
    requireTrue(*clause[0] == *expected[0]);
    requireTrue(*clause[1] == *expected[1]);
}

TEST_CASE_METHOD(setUpMcp, "pattern clauses, assign, assign") {
    query = "pattern a(v,_) and a(v,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    ClauseParser cp(pqlTokenScanner, declarationList);
    clause = cp.parse();
    expected.push_back(std::make_unique<AssignPattern>(std::move(synonymVariable1), std::move(wildcard1), "a"));
    expected.push_back(std::make_unique<AssignPattern>(std::move(synonymVariable2), std::move(wildcard2), "a"));
    requireTrue(*clause[0] == *expected[0]);
    requireTrue(*clause[1] == *expected[1]);
}

TEST_CASE_METHOD(setUpMcp, "pattern clauses, if, assign") {
    query = "pattern ifs(v,_,_) and a(v,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    ClauseParser cp(pqlTokenScanner, declarationList);
    clause = cp.parse();
    expected.push_back(std::make_unique<IfPattern>(std::move(synonymVariable1), "ifs"));
    expected.push_back(std::make_unique<AssignPattern>(std::move(synonymVariable2), std::move(wildcard2), "a"));
    requireTrue(*clause[0] == *expected[0]);
    requireTrue(*clause[1] == *expected[1]);
}

TEST_CASE_METHOD(setUpMcp, "pattern clauses, if, while") {
    query = "pattern ifs(v,_,_) and w(v,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    ClauseParser cp(pqlTokenScanner, declarationList);
    clause = cp.parse();
    expected.push_back(std::make_unique<IfPattern>(std::move(synonymVariable1), "ifs"));
    expected.push_back(std::make_unique<WhilePattern>(std::move(synonymVariable2), "w"));
    requireTrue(*clause[0] == *expected[0]);
    requireTrue(*clause[1] == *expected[1]);
}

TEST_CASE_METHOD(setUpMcp, "with clauses, withEnt, withEnt") {
    query = "with \"x\" = p.procName and cl.procName = v.varName";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    ClauseParser cp(pqlTokenScanner, declarationList);
    clause = cp.parse();
    expected.push_back(std::make_unique<WithEntClause>(std::move(identStr), std::move(synonymProcedure)));
    expected.push_back(std::make_unique<WithEntClause>(std::move(synonymCall), std::move(synonymVariable1)));
    requireTrue(*clause[0] == *expected[0]);
    requireTrue(*clause[1] == *expected[1]);
}

TEST_CASE_METHOD(setUpMcp, "with clauses, withNum, withNum") {
    query = "with 1 = c.value and s.stmt# = r.stmt#";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    ClauseParser cp(pqlTokenScanner, declarationList);
    clause = cp.parse();
    expected.push_back(std::make_unique<WithNumClause>(std::move(statementNumber1), std::move(synonymConstant)));
    expected.push_back(std::make_unique<WithNumClause>(std::move(synonymStatement1), std::move(synonymRead)));
    requireTrue(*clause[0] == *expected[0]);
    requireTrue(*clause[1] == *expected[1]);
}

TEST_CASE_METHOD(setUpMcp, "with clauses, withEnt, withNum") {
    query = "with r.varName = pn.varName and pn.stmt# = cl.stmt#";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    ClauseParser cp(pqlTokenScanner, declarationList);
    clause = cp.parse();
    expected.push_back(std::make_unique<WithEntClause>(std::move(synonymRead), std::move(synonymPrint1)));
    expected.push_back(std::make_unique<WithNumClause>(std::move(synonymPrint2), std::move(synonymCall)));
    requireTrue(*clause[0] == *expected[0]);
    requireTrue(*clause[1] == *expected[1]);
}

TEST_CASE_METHOD(setUpMcp, "with clauses, withNum, withEnt") {
    query = "with ifs.stmt# = a.stmt# and p.procName = cl.procName";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    ClauseParser cp(pqlTokenScanner, declarationList);
    clause = cp.parse();
    expected.push_back(std::make_unique<WithNumClause>(std::move(synonymIf), std::move(synonymAssign)));
    expected.push_back(std::make_unique<WithEntClause>(std::move(synonymProcedure), std::move(synonymCall)));
    requireTrue(*clause[0] == *expected[0]);
    requireTrue(*clause[1] == *expected[1]);
}

TEST_CASE_METHOD(setUpMcp, "with, such that, syntax error") {
    query = "Select a with ifs.stmt# = a.stmt# and Modifies(1,v)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(std::move(lexer));
    requireTrue(!sv->validateQuery());
}
