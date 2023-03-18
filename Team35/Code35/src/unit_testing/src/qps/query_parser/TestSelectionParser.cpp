#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/query_parser/selection_parser/SelectionParser.h"
#include "commons/lexer/LexerFactory.h"
#include "qps/clause/select_clause/SingleSynonymSelectClause.h"
#include "qps/query_parser/QuerySyntaxValidator.h"
#include "qps/pql/AttrRef.h"
#include "qps/query_parser/helper.h"

TEST_CASE("Selection parser") {
    std::string query = "Select s";
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;
    declarationList.insert({"s", Synonym::DesignEntity::STMT});
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    std::unique_ptr<SelectClause> sc = sp.parse();
    std::unique_ptr<Synonym> s = std::make_unique<Synonym>(Synonym::DesignEntity::STMT, "s");
    requireTrue(*sc == *std::make_unique<SingleSynonymSelectClause>(std::move(s)));
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

TEST_CASE("Selection parser, attrRef") {
    std::string query = "Select s.stmt#";
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;
    declarationList.insert({"s", Synonym::DesignEntity::STMT});
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    std::unique_ptr<SelectClause> sc = sp.parse();
    std::unique_ptr<AttrRef> attrRef = std::make_unique<AttrRef>(Synonym::DesignEntity::STMT, "s", STMT_KEYWORD);
    std::unique_ptr<SelectClause> expected = std::make_unique<SingleSynonymSelectClause>(std::move(attrRef));
    requireTrue(*sc == *expected);
}
