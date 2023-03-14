#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/query_parser/selection_parser/SelectionParser.h"
#include "commons/lexer/LexerFactory.h"
#include "qps/clause/select_clause/SingleSynonymSelectClause.h"

TEST_CASE("Selection parser") {
    std::string query = "Select s";
    std::unordered_map<std::string, Synonym::DesignEntity> declarationList;
    declarationList.insert({"s", Synonym::DesignEntity::STMT});
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    PQLTokenScanner pqlTokenScanner(std::move(lexer));
    SelectionParser sp(pqlTokenScanner, declarationList);
    std::unique_ptr<SelectClause> sc = sp.parse();
    Synonym s(Synonym::DesignEntity::STMT, "s");
    requireTrue(*sc == *std::make_unique<SingleSynonymSelectClause>(s));
}
