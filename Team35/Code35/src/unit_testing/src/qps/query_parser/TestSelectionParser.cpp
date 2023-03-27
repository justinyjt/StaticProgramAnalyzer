#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/query_parser/selection_parser/SelectionParser.h"
#include "commons/lexer/LexerFactory.h"
#include "qps/clause/select_clause/MultipleSynonymSelectClause.h"
#include "qps/query_parser/QuerySyntaxValidator.h"
#include "qps/pql/AttrRef.h"
#include "qps/query_parser/helper.h"
#include "qps/clause/select_clause/BooleanSelectClause.h"
#include "qps/query_exceptions/SemanticException.h"

TEST_CASE("Select synonym") {
    std::string query = "Select s";
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;
    declarationList.insert({"s", Synonym::DesignEntity::STMT});
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    std::unique_ptr<SelectClause> sc = sp.parse();
    std::vector<std::unique_ptr<Synonym>> synonyms;
    std::unique_ptr<Synonym> s = std::make_unique<Synonym>(Synonym::DesignEntity::STMT, "s");
    synonyms.push_back(std::move(s));
    requireTrue(*sc == *std::make_unique<MultipleSynonymSelectClause>(std::move(synonyms)));
}

TEST_CASE("Select BOOLEAN declared as synonym") {
    std::string query = "Select BOOLEAN";
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;
    declarationList.insert({"BOOLEAN", Synonym::DesignEntity::STMT});
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    std::unique_ptr<SelectClause> sc = sp.parse();
    std::vector<std::unique_ptr<Synonym>> synonyms;
    std::unique_ptr<Synonym> s = std::make_unique<Synonym>(Synonym::DesignEntity::STMT, "BOOLEAN");
    synonyms.push_back(std::move(s));
    requireTrue(*sc == *std::make_unique<MultipleSynonymSelectClause>(std::move(synonyms)));
}

TEST_CASE("Select BOOLEAN") {
    std::string query = "Select BOOLEAN";
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    std::unique_ptr<SelectClause> sc = sp.parse();
    requireTrue(*sc == *std::make_unique<BooleanSelectClause>());
}

TEST_CASE("Select attrRef") {
    std::string query = "Select s.stmt#";
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;
    declarationList.insert({"s", Synonym::DesignEntity::STMT});
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    std::unique_ptr<SelectClause> sc = sp.parse();
    std::vector<std::unique_ptr<Synonym>> synonyms;
    std::unique_ptr<AttrRef> attrRef = std::make_unique<AttrRef>(Synonym::DesignEntity::STMT, "s", STMT_KEYWORD);
    synonyms.push_back(std::move(attrRef));
    requireTrue(*sc == *std::make_unique<MultipleSynonymSelectClause>(std::move(synonyms)));
}

TEST_CASE("Select tuple") {
    std::string query = "Select <s.stmt#, v>";
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;
    declarationList.insert({"s", Synonym::DesignEntity::STMT});
    declarationList.insert({"v", Synonym::DesignEntity::VARIABLE});
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    std::unique_ptr<SelectClause> sc = sp.parse();
    std::vector<std::unique_ptr<Synonym>> synonyms;
    std::unique_ptr<AttrRef> attrRef = std::make_unique<AttrRef>(Synonym::DesignEntity::STMT, "s", STMT_KEYWORD);
    std::unique_ptr<Synonym> synonym = std::make_unique<Synonym>(Synonym::DesignEntity::VARIABLE, "v");
    synonyms.push_back(std::move(attrRef));
    synonyms.push_back(std::move(synonym));
    requireTrue(*sc == *std::make_unique<MultipleSynonymSelectClause>(std::move(synonyms)));
}

TEST_CASE("Select tuple, with BOOLEAN as synonym") {
    std::string query = "Select <s.stmt#, BOOLEAN>";
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;
    declarationList.insert({"s", Synonym::DesignEntity::STMT});
    declarationList.insert({"BOOLEAN", Synonym::DesignEntity::VARIABLE});
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    std::unique_ptr<SelectClause> sc = sp.parse();
    std::vector<std::unique_ptr<Synonym>> synonyms;
    std::unique_ptr<AttrRef> attrRef = std::make_unique<AttrRef>(Synonym::DesignEntity::STMT, "s", STMT_KEYWORD);
    std::unique_ptr<Synonym> synonym = std::make_unique<Synonym>(Synonym::DesignEntity::VARIABLE, "BOOLEAN");
    synonyms.push_back(std::move(attrRef));
    synonyms.push_back(std::move(synonym));
    requireTrue(*sc == *std::make_unique<MultipleSynonymSelectClause>(std::move(synonyms)));
}

TEST_CASE("Select tuple, with BOOLEAN, semantic error") {
    std::string query = "Select <s.stmt#, BOOLEAN>";
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;
    declarationList.insert({"s", Synonym::DesignEntity::STMT});
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    requireThrowAs<SemanticException>([&sp]() {
        std::unique_ptr<SelectClause> sc = sp.parse();
    });
}

TEST_CASE("Select tuple - syn, syn") {
    std::string query = "Select <s, v>";
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(std::move(lexer));
    requireTrue(sv->validateQuery());
}

TEST_CASE("Select tuple syn.procName, syn.varName") {
    std::string query = "Select <s.procName, v.varName, s, s.value, s.stmt#>";
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    std::unique_ptr<QuerySyntaxValidator> sv = std::make_unique<QuerySyntaxValidator>(std::move(lexer));
    requireTrue(sv->validateQuery());
}
