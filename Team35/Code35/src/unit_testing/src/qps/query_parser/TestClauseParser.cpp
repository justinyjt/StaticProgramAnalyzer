#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/query_parser/clause_parser/ClauseParser.h"
#include "commons/lexer/LexerFactory.h"

TEST_CASE("Clause parser") {
    std::string query = "such that Modify(1,v)";
    ClauseParser cp;
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);
    std::vector<Synonym> synonyms;
    synonyms.push_back(Synonym(Synonym::DesignEntity::STMT, "v"));
    std::vector<Clause*> clauses = cp.parse(tokenValidator, synonyms);
}
