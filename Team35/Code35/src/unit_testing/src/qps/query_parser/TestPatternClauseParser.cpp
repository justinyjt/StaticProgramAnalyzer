#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/pql/Synonym.h"
#include "qps/clause/Clause.h"
#include "qps/query_parser/clause_parser/pattern_clause_parser/PatternClauseParser.h"
#include "commons/lexer/LexerFactory.h"
#include "qps/pql/Ident.h"
#include "qps/pql/Expression.h"
#include "qps/query_exceptions/SyntaxException.h"
#include "qps/clause/one_arg_clause//WhilePattern.h"
#include "qps/clause/one_arg_clause/IfPattern.h"

class setUp {
 public:
    setUp() {
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
        expr = std::make_unique<Expression>("x", false);
        exprWildcardVarName = std::make_unique<Expression>("x", true);
        exprWildcardConstValue = std::make_unique<Expression>("1", true);
        wildcard1 = std::make_unique<Wildcard>();
        wildcard2 = std::make_unique<Wildcard>();
    }

    ~setUp() {
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

    std::unique_ptr<Clause> clause;
    std::unique_ptr<AssignPattern> patternAssign;
    std::unique_ptr<WhilePattern> patternWhile;
    std::unique_ptr<IfPattern> patternIf;
    std::unique_ptr<ILexer> lexer;
};

TEST_CASE_METHOD(setUp, "variable, wildcard") {
    query = "pattern a(v, _)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    clause = pcp.parse();
    patternAssign = std::make_unique<AssignPattern>(std::move(synonymVariable), std::move(wildcard1), "a");
    requireTrue(*clause == *patternAssign);
}

TEST_CASE_METHOD(setUp, "variable, variable name and wildcard") {
    query = "pattern a(v,_\"x\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    clause = pcp.parse();
    patternAssign = std::make_unique<AssignPattern>(std::move(synonymVariable), std::move(exprWildcardVarName), "a");
    requireTrue(*clause == *patternAssign);
}

TEST_CASE_METHOD(setUp, "variable, constant value and wildcard") {
    query = "pattern a(v,_\"1\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    clause = pcp.parse();
    patternAssign = std::make_unique<AssignPattern>(std::move(synonymVariable), std::move(exprWildcardConstValue), "a");
    requireTrue(*clause == *patternAssign);
}

TEST_CASE_METHOD(setUp, "wildcard and wildcard") {
    query = "pattern a(_,_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    clause = pcp.parse();
    patternAssign = std::make_unique<AssignPattern>(std::move(wildcard1), std::move(wildcard2), "a");
    requireTrue(*clause == *patternAssign);
}

TEST_CASE_METHOD(setUp, "wildcard and variable name with wildcard") {
    query = "pattern a(_,_\"x\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    clause = pcp.parse();
    patternAssign = std::make_unique<AssignPattern>(std::move(wildcard1), std::move(exprWildcardVarName), "a");
    requireTrue(*clause == *patternAssign);
}

TEST_CASE_METHOD(setUp, "invalid LHS constant and variable name with wildcard") {
    query = "pattern a(\"1\",_\"x\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    requireThrowAs<SyntaxException>([&pcp]() {
        pcp.parse();
    });
}

TEST_CASE_METHOD(setUp, "invalid LHS varName and variable name with wildcard") {
    query = "pattern a(\"+x\",_\"x\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    requireThrowAs<SyntaxException>([&pcp]() {
        pcp.parse();
    });
}

TEST_CASE_METHOD(setUp, "ident and invalid RHS expr 1") {
    query = "pattern a(\"x\",_\"x+\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    requireThrowAs<SyntaxException>([&pcp]() {
        pcp.parse();
    });
}

TEST_CASE_METHOD(setUp, "ident and invalid RHS expr 2") {
    query = "pattern a(\"x\",_\"x+\"_)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    requireThrowAs<SyntaxException>([&pcp]() {
        pcp.parse();
    });
}

TEST_CASE_METHOD(setUp, "while pattern, variable") {
    query = "pattern w(v, _)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    clause = pcp.parse();
    patternWhile = std::make_unique<WhilePattern>(std::move(synonymVariable), "w");
    requireTrue(*clause == *patternWhile);
}

TEST_CASE_METHOD(setUp, "if pattern, variable") {
    query = "pattern ifs(v, _, _)";
    lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    PatternClauseParser pcp(pqlTokenScanner, declarationList);
    clause = pcp.parse();
    patternIf = std::make_unique<IfPattern>(std::move(synonymVariable), "ifs");
    requireTrue(*clause == *patternIf);
}
