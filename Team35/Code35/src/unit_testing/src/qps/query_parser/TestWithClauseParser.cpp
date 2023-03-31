#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/query_parser/clause_parser/ClauseParser.h"
#include "commons/lexer/LexerFactory.h"
#include "qps/pql/PQLNumber.h"
#include "qps/pql/Ident.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "commons/expr_parser/ExprParser.h"
#include "qps/clause/OptimisableClause.h"
#include "qps/clause/two_arg_clause/WithEntClause.h"
#include "qps/clause/two_arg_clause/WithNumClause.h"

class setUpWcp {
 public:
    setUpWcp() {
        // Set up code
        query = "";
        declarationList.insert({"p", Synonym::DesignEntity::PROCEDURE});
        declarationList.insert({"pn", Synonym::DesignEntity::PRINT});
        declarationList.insert({"a", Synonym::DesignEntity::ASSIGN});
        declarationList.insert({"v", Synonym::DesignEntity::VARIABLE});
        declarationList.insert({"c", Synonym::DesignEntity::CONSTANT});

        synonymProcedure = std::make_unique<Synonym>(Synonym::DesignEntity::PROCEDURE, "p");
        synonymPrint = std::make_unique<Synonym>(Synonym::DesignEntity::PRINT, "pn");
        synonymAssign = std::make_unique<Synonym>(Synonym::DesignEntity::ASSIGN, "a");
        synonymWhile = std::make_unique<Synonym>(Synonym::DesignEntity::WHILE, "w");
        synonymIf = std::make_unique<Synonym>(Synonym::DesignEntity::IF, "ifs");
        synonymVariable = std::make_unique<Synonym>(Synonym::DesignEntity::VARIABLE, "v");
        synonymConstant = std::make_unique<Synonym>(Synonym::DesignEntity::CONSTANT, "c");

        ident1 = std::make_unique<Ident>("x");
        ident2 = std::make_unique<Ident>("x");
        statementNumber1 = std::make_unique<PQLNumber>("1");
        statementNumber2 = std::make_unique<PQLNumber>("1");
        std::string exprStr1 = "x";
        std::unique_ptr<ILexer> lxr1 =
            LexerFactory::createLexer(exprStr1, LexerFactory::LexerType::Expression);
        TokenScanner scanner1(std::move(lxr1));
        ExprParser parser1(scanner1);
        ASSIGN_PAT_RIGHT pattern1 = parser1.parseExpr();
        std::string exprStr2 = "1";
        std::unique_ptr<ILexer> lxr2 =
            LexerFactory::createLexer(exprStr2, LexerFactory::LexerType::Expression);
        TokenScanner scanner2(std::move(lxr2));
        ExprParser parser2(scanner2);
        ASSIGN_PAT_RIGHT pattern2 = parser2.parseExpr();
        expr = std::make_unique<Expression>(pattern1, false);
        exprWildcardVarName = std::make_unique<Expression>(pattern1, true);
        exprWildcardConstValue = std::make_unique<Expression>(pattern2, true);
        wildcard1 = std::make_unique<Wildcard>();
        wildcard2 = std::make_unique<Wildcard>();
    }

    ~setUpWcp() {
        // Tear down code
    }

    // Declarations
    std::string query;
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;

    std::unique_ptr<Synonym> synonymProcedure;
    std::unique_ptr<Synonym> synonymPrint;
    std::unique_ptr<Synonym> synonymAssign;
    std::unique_ptr<Synonym> synonymWhile;
    std::unique_ptr<Synonym> synonymIf;
    std::unique_ptr<Synonym> synonymVariable;
    std::unique_ptr<Synonym> synonymConstant;

    std::unique_ptr<Ident> ident1;
    std::unique_ptr<Ident> ident2;
    std::unique_ptr<PQLNumber> statementNumber1;
    std::unique_ptr<PQLNumber> statementNumber2;
    std::unique_ptr<Expression> expr;
    std::unique_ptr<Expression> exprWildcardVarName;
    std::unique_ptr<Expression> exprWildcardConstValue;
    std::unique_ptr<Wildcard> wildcard1;
    std::unique_ptr<Wildcard> wildcard2;

    std::vector<std::unique_ptr<OptimisableClause>> clause;
    std::unique_ptr<WithEntClause> withEntClause;
    std::unique_ptr<WithNumClause> withNumClause;
    std::unique_ptr<ILexer> lexer;
};

TEST_CASE_METHOD(setUpWcp, "ident, ident") {
    query = "with \"x\" = \"x\"";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    WithClauseParser wcp(pqlTokenScanner, declarationList);
    clause = wcp.parse();
    withEntClause = std::make_unique<WithEntClause>(std::move(ident1), std::move(ident2));
    requireTrue(*clause.front() == *withEntClause);
}

TEST_CASE_METHOD(setUpWcp, "ident, int, semantic error") {
    query = "with \"x\" = 1";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    WithClauseParser wcp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&wcp]() {
      wcp.parse();
    });
}

TEST_CASE_METHOD(setUpWcp, "ident, procedure.procName") {
    query = "with \"x\" = p.procName";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    WithClauseParser wcp(pqlTokenScanner, declarationList);
    clause = wcp.parse();
    withEntClause = std::make_unique<WithEntClause>(std::move(ident1), std::move(synonymProcedure));
    requireTrue(*clause.front() == *withEntClause);
}

TEST_CASE_METHOD(setUpWcp, "ident, procedure.value, semantic error") {
    query = "with \"x\" = p.value";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    WithClauseParser wcp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&wcp]() {
      wcp.parse();
    });
}

TEST_CASE_METHOD(setUpWcp, "int, constant") {
    query = "with 1 = 1";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    WithClauseParser wcp(pqlTokenScanner, declarationList);
    clause = wcp.parse();
    withNumClause = std::make_unique<WithNumClause>(std::move(statementNumber1), std::move(statementNumber2));
    requireTrue(*clause.front() == *withNumClause);
}

TEST_CASE_METHOD(setUpWcp, "procedure.procName, print.procName, semantic error") {
    query = "with p.procName = pn.procName";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    WithClauseParser wcp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&wcp]() {
      wcp.parse();
    });
}

TEST_CASE_METHOD(setUpWcp, "procedure.procName, print.varName") {
    query = "with p.procName = pn.varName";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    WithClauseParser wcp(pqlTokenScanner, declarationList);
    clause = wcp.parse();
    withEntClause = std::make_unique<WithEntClause>(std::move(synonymProcedure), std::move(synonymPrint));
    requireTrue(*clause.front() == *withEntClause);
}

TEST_CASE_METHOD(setUpWcp, "print.stmt, constant.value") {
    query = "with pn.stmt# = c.value";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    WithClauseParser wcp(pqlTokenScanner, declarationList);
    clause = wcp.parse();
    withNumClause = std::make_unique<WithNumClause>(std::move(synonymPrint), std::move(synonymConstant));
    requireTrue(*clause.front() == *withNumClause);
}
