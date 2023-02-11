#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/query_parser/declaration_parser/DeclarationParser.h"
#include "qps/clause/Clause.h"
#include "qps/pql/StatementNumber.h"
#include "commons/lexer/LexerFactory.h"
#include "qps/query_parser/selection_parser/SelectionParser.h"
#include "qps/query_parser/clause_parser/ClauseParser.h"

TEST_CASE("1. Query parser") {
    std::string query = "variable v, x; assign a, b, c; read y; Select c such that Modifies(2,v) pattern a ( _ , \"x\")";
    DeclarationParser dp;
    SelectionParser sp;
    ClauseParser cp;
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(query, LexerFactory::LexerType::Pql);
    TokenValidator tokenValidator(lexer);
    std::vector<Synonym> declarationList = dp.parse(tokenValidator);
    requireEqual(declarationList.front(), Synonym(Synonym::DesignEntity::VARIABLE, "v"));

    Synonym selectedSynonym = sp.parse(tokenValidator, declarationList);
    requireEqual(selectedSynonym, Synonym(Synonym::DesignEntity::ASSIGN, "c"));

    std::vector<Clause *> clauses = cp.parse(tokenValidator, declarationList);

    Clause *c1 = clauses.front();
    requireTrue((c1->first).getValue() == "2");
    requireTrue((c1->second).getValue() == "v");

    Clause *c2 = clauses.back();
    requireTrue((c2->first).getValue() == "_");
    requireTrue((c2->second).getValue() == "x");
}
