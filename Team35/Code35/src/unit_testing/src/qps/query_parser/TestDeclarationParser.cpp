#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/query_parser/declaration_parser/DeclarationParser.h"
#include "commons/lexer/ILexer.h"
#include "commons/lexer/LexerFactory.h"

TEST_CASE("Declaration parser; 1 design entity") {
    std::string query = "variable v;";
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    DeclarationParser dp(pqlTokenScanner, declarationList);
    declarationList = dp.parse();
    std::unordered_map<std::string, Synonym::DesignEntity> expected;
    expected.insert({"v", Synonym::DesignEntity::VARIABLE});
    requireEqual(declarationList, expected);
}

TEST_CASE("Declaration parser; 1 design entity of each type") {
    std::string query = "stmt s; read r; print pr; call cl; while w; "
                        "if ifs; assign a; variable v; constant c; procedure p;";
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    DeclarationParser dp(pqlTokenScanner, declarationList);
    declarationList = dp.parse();
    std::unordered_map<std::string, Synonym::DesignEntity> expected;
    expected.insert({"s", Synonym::DesignEntity::STMT});
    expected.insert({"r", Synonym::DesignEntity::READ});
    expected.insert({"pr", Synonym::DesignEntity::PRINT});
    expected.insert({"cl", Synonym::DesignEntity::CALL});
    expected.insert({"w", Synonym::DesignEntity::WHILE});
    expected.insert({"ifs", Synonym::DesignEntity::IF});
    expected.insert({"a", Synonym::DesignEntity::ASSIGN});
    expected.insert({"v", Synonym::DesignEntity::VARIABLE});
    expected.insert({"c", Synonym::DesignEntity::CONSTANT});
    expected.insert({"p", Synonym::DesignEntity::PROCEDURE});
    requireEqual(declarationList, expected);
}

TEST_CASE("Declaration parser; multiple design entity of each type") {
    std::string query = "stmt s1, s2; read r1, r2;";
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    DeclarationParser dp(pqlTokenScanner, declarationList);
    declarationList = dp.parse();
    std::unordered_map<std::string, Synonym::DesignEntity> expected;
    expected.insert({"s1", Synonym::DesignEntity::STMT});
    expected.insert({"s2", Synonym::DesignEntity::STMT});
    expected.insert({"r1", Synonym::DesignEntity::READ});
    expected.insert({"r2", Synonym::DesignEntity::READ});
    requireEqual(declarationList, expected);
}

TEST_CASE("Declaration parser; same synonym names and terminals") {
    std::string query = "stmt stmt; assign pattern;";
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    DeclarationParser dp(pqlTokenScanner, declarationList);
    declarationList = dp.parse();
    std::unordered_map<std::string, Synonym::DesignEntity> expected;
    expected.insert({"stmt", Synonym::DesignEntity::STMT});
    expected.insert({"pattern", Synonym::DesignEntity::ASSIGN});
    requireEqual(declarationList, expected);
}

TEST_CASE("Declaration parser; multiple design entity of each type with repeats") {
    std::string query = "stmt s1, s1; read r1, r2;";
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    DeclarationParser dp(pqlTokenScanner, declarationList);
    requireThrow([&dp]() {
        dp.parse();
    });
}

TEST_CASE("Declaration parser; multiple design entity of across types with repeats") {
    std::string query = "stmt s1, s2; read s1, s2;";
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    DeclarationParser dp(pqlTokenScanner, declarationList);
    requireThrow([&dp]() {
        dp.parse();
    });
}