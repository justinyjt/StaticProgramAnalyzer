#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/query_parser/selection_parser/SelectionParser.h"
#include "commons/lexer/LexerFactory.h"
#include "qps/clause/SingleSynonymSelectClause.h"

TEST_CASE("Selection parser") {
    std::string query = "Select s";
    SelectionParser sp;
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);
    std::vector<Synonym> synonyms;
    synonyms.emplace_back(Synonym(Synonym::DesignEntity::STMT, "s"));
    std::unique_ptr<SelectClause> sc = sp.parse(tokenValidator, synonyms);
    Synonym s(Synonym::DesignEntity::STMT, "s");
    requireTrue(*sc == *std::make_unique<SingleSynonymSelectClause>(s));
}