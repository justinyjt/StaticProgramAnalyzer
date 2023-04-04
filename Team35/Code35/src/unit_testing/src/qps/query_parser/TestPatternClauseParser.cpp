#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/pql/Synonym.h"
#include "qps/clause/OptimisableClause.h"
#include "qps/query_parser/clause_parser/pattern_clause_parser/PatternClauseParser.h"
#include "commons/lexer/LexerFactory.h"
#include "qps/pql/Ident.h"
#include "qps/pql/Expression.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "qps/clause/one_arg_clause/OneArgClause.h"
#include "qps/query_parser/QuerySyntaxValidator.h"
#include "commons/expr_parser/ExprParser.h"

class setUpPcp {
 public:
    setUpPcp() {
        // Set up code
        query = "";
        declarationList.insert({"p", Synonym::DesignEntity::PROCEDURE});
        declarationList.insert({"a", Synonym::DesignEntity::ASSIGN});
        declarationList.insert({"w", Synonym::DesignEntity::WHILE});
        declarationList.insert({"ifs", Synonym::DesignEntity::IF});
        declarationList.insert({"v", Synonym::DesignEntity::VARIABLE});
        declarationList.insert({"c", Synonym::DesignEntity::CONSTANT});

        synonymProcedure = std::make_unique<Synonym>(Synonym::DesignEntity::PROCEDURE, "p");
        synonymAssign = std::make_unique<Synonym>(Synonym::DesignEntity::ASSIGN, "a");
        synonymWhile = std::make_unique<Synonym>(Synonym::DesignEntity::WHILE, "w");
        synonymIf = std::make_unique<Synonym>(Synonym::DesignEntity::IF, "ifs");
        synonymVariable = std::make_unique<Synonym>(Synonym::DesignEntity::VARIABLE, "v");
        synonymConstant = std::make_unique<Synonym>(Synonym::DesignEntity::CONSTANT, "c");

        ident = std::make_unique<Ident>("x");
        std::string exprStr = "x";
        std::unique_ptr<ILexer> lxr =
            LexerFactory::createLexer(exprStr, LexerFactory::LexerType::Expression);
        TokenScanner scanner(std::move(lxr));
        ExprParser parser(scanner);
        ASSIGN_PAT_RIGHT pattern = parser.parseExpr();
        exprStr = "1";
        lxr =
            LexerFactory::createLexer(exprStr, LexerFactory::LexerType::Expression);
        TokenScanner scanner2(std::move(lxr));
        ExprParser parser2(scanner2);
        ASSIGN_PAT_RIGHT pattern2 = parser2.parseExpr();
        expr = std::make_unique<Expression>(pattern, false);
        exprWildcardVarName = std::make_unique<Expression>(pattern, true);
        exprWildcardConstValue = std::make_unique<Expression>(pattern2, true);
        wildcard1 = std::make_unique<Wildcard>();
        wildcard2 = std::make_unique<Wildcard>();
    }

    ~setUpPcp() {
        // Tear down code
    }

    // Declarations
    std::string query;
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;

    std::unique_ptr<Synonym> synonymProcedure;
    std::unique_ptr<Synonym> synonymAssign;
    std::unique_ptr<Synonym> synonymWhile;
    std::unique_ptr<Synonym> synonymIf;
    std::unique_ptr<Synonym> synonymVariable;
    std::unique_ptr<Synonym> synonymConstant;

    std::unique_ptr<Ident> ident;
    std::unique_ptr<Expression> expr;
    std::unique_ptr<Expression> exprWildcardVarName;
    std::unique_ptr<Expression> exprWildcardConstValue;
    std::unique_ptr<Wildcard> wildcard1;
    std::unique_ptr<Wildcard> wildcard2;

    std::vector<std::unique_ptr<OptimisableClause>> clause;
    std::unique_ptr<AssignPatternClause> patternAssign;
    std::unique_ptr<WhilePattern> patternWhile;
    std::unique_ptr<IfPattern> patternIf;
    std::unique_ptr<ILexer> lexer;
};

TEST_CASE_METHOD(setUpPcp, "invalid, not a pattern synonym") {
    query = "pattern p(v, _)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&pcp]() {
        pcp.parse();
    });
}

TEST_CASE_METHOD(setUpPcp, "variable, wildcard") {
    query = "pattern a(v, _)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    clause = pcp.parse();
    patternAssign = std::make_unique<AssignPatternClause>(std::move(synonymVariable), std::move(wildcard1), "a");
    requireTrue(*clause.front() == *patternAssign);
}

TEST_CASE_METHOD(setUpPcp, "variable, variable name and wildcard") {
    query = "pattern a(v,_\"x\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    clause = pcp.parse();
    patternAssign = std::make_unique<AssignPatternClause>(
        std::move(synonymVariable), std::move(exprWildcardVarName), "a");
    requireTrue(*clause.front() == *patternAssign);
}

TEST_CASE_METHOD(setUpPcp, "variable, constant value and wildcard") {
    query = "pattern a(v,_\"1\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    clause = pcp.parse();
    patternAssign = std::make_unique<AssignPatternClause>(
        std::move(synonymVariable), std::move(exprWildcardConstValue), "a");
    requireTrue(*clause.front() == *patternAssign);
}

TEST_CASE_METHOD(setUpPcp, "wildcard and wildcard") {
    query = "pattern a(_,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    clause = pcp.parse();
    patternAssign = std::make_unique<AssignPatternClause>(std::move(wildcard1), std::move(wildcard2), "a");
    requireTrue(*clause.front() == *patternAssign);
}

TEST_CASE_METHOD(setUpPcp, "wildcard and variable name with wildcard") {
    query = "pattern a(_,_\"x\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    clause = pcp.parse();
    patternAssign = std::make_unique<AssignPatternClause>(std::move(wildcard1), std::move(exprWildcardVarName), "a");
    requireTrue(*clause.front() == *patternAssign);
}

TEST_CASE_METHOD(setUpPcp, "pattern valid entRef ident") {
    query = "pattern a(\"x\",_\"x\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    clause = pcp.parse();
    patternAssign = std::make_unique<AssignPatternClause>(std::move(ident), std::move(exprWildcardVarName), "a");
    requireTrue(*clause.front() == *patternAssign);
}

TEST_CASE_METHOD(setUpPcp, "while pattern, variable") {
    query = "pattern w(v, _)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    clause = pcp.parse();
    patternWhile = std::make_unique<WhilePattern>(std::move(synonymVariable), "w");
    requireTrue(*clause.front() == *patternWhile);
}

TEST_CASE_METHOD(setUpPcp, "while pattern, wildcard") {
    query = "pattern w(_, _)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    clause = pcp.parse();
    patternWhile = std::make_unique<WhilePattern>(std::move(wildcard1), "w");
    requireTrue(*clause.front() == *patternWhile);
}

TEST_CASE_METHOD(setUpPcp, "while pattern, ident") {
    query = "pattern w(\"x\", _)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    clause = pcp.parse();
    patternWhile = std::make_unique<WhilePattern>(std::move(ident), "w");
    requireTrue(*clause.front() == *patternWhile);
}

TEST_CASE_METHOD(setUpPcp, "if pattern, variable") {
    query = "pattern ifs(v, _, _)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    clause = pcp.parse();
    patternIf = std::make_unique<IfPattern>(std::move(synonymVariable), "ifs");
    requireTrue(*clause.front() == *patternIf);
}

TEST_CASE_METHOD(setUpPcp, "if pattern, invalid 2 args only") {
    query = "pattern ifs(v, _)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&pcp]() {
        pcp.parse();
    });
}

TEST_CASE_METHOD(setUpPcp, "while pattern, assign pattern syntax, semantic error") {
    query = "pattern w(_, \"a\")";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&pcp]() {
      pcp.parse();
    });
}
