#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/query_parser/declaration_parser/DeclarationParser.h"
#include "commons/lexer/ILexer.h"
#include "commons/lexer/LexerFactory.h"

TEST_CASE("Declaration parser; 1 design entity") {
    std::string query = "variable v;";
    DeclarationParser dp;
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);
    std::vector<Synonym> declarationList = dp.parse(tokenValidator);
    requireEqual(declarationList.at(0), Synonym(Synonym::DesignEntity::VARIABLE, "v"));
}

TEST_CASE("Declaration parser; 1 design entity of each type") {
    std::string query = "stmt s; read r; print pr; call cl; while w; "
                        "if ifs; assign a; variable v; constant c; procedure p;";
    DeclarationParser dp;
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);
    std::vector<Synonym> declarationList = dp.parse(tokenValidator);
    requireEqual(declarationList.at(0), Synonym(Synonym::DesignEntity::STMT, "s"));
    requireEqual(declarationList.at(1), Synonym(Synonym::DesignEntity::READ, "r"));
    requireEqual(declarationList.at(2), Synonym(Synonym::DesignEntity::PRINT, "pr"));
    requireEqual(declarationList.at(3), Synonym(Synonym::DesignEntity::CALL, "cl"));
    requireEqual(declarationList.at(4), Synonym(Synonym::DesignEntity::WHILE, "w"));
    requireEqual(declarationList.at(5), Synonym(Synonym::DesignEntity::IF, "ifs"));
    requireEqual(declarationList.at(6), Synonym(Synonym::DesignEntity::ASSIGN, "a"));
    requireEqual(declarationList.at(7), Synonym(Synonym::DesignEntity::VARIABLE, "v"));
    requireEqual(declarationList.at(8), Synonym(Synonym::DesignEntity::CONSTANT, "c"));
    requireEqual(declarationList.at(9), Synonym(Synonym::DesignEntity::PROCEDURE, "p"));
}

TEST_CASE("Declaration parser; multiple design entity of each type") {
    std::string query = "stmt s1, s2; read r1, r2;";
    DeclarationParser dp;
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);
    std::vector<Synonym> declarationList = dp.parse(tokenValidator);
    requireEqual(declarationList.at(0), Synonym(Synonym::DesignEntity::STMT, "s1"));
    requireEqual(declarationList.at(1), Synonym(Synonym::DesignEntity::STMT, "s2"));
    requireEqual(declarationList.at(2), Synonym(Synonym::DesignEntity::READ, "r1"));
    requireEqual(declarationList.at(3), Synonym(Synonym::DesignEntity::READ, "r2"));
}

TEST_CASE("Declaration parser; same synonym names and terminals") {
    std::string query = "stmt stmt; assign pattern;";
    DeclarationParser dp;
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);
    std::vector<Synonym> declarationList = dp.parse(tokenValidator);
    requireEqual(declarationList.at(0), Synonym(Synonym::DesignEntity::STMT, "stmt"));
    requireEqual(declarationList.at(1), Synonym(Synonym::DesignEntity::ASSIGN, "pattern"));
}


TEST_CASE("Declaration parser; multiple design entity of each type with repeats") {
    std::string query = "stmt s1, s1; read r1, r2;";
    DeclarationParser dp;
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);
    requireThrow([&tokenValidator, &dp]() {
        dp.parse(tokenValidator);
    });
}
