#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/query_parser/declaration_parser/DeclarationParser.h"
#include "qps/clause/Clause.h"
#include "qps/clause/relationship/Modifies.h"
#include "qps/pql/StatementNumber.h"
#include "qps/pql/Expression.h"
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

    // Perform parsing
    std::vector<Clause *> clauses = cp.parse(tokenValidator, declarationList);

    requireTrue(clauses.size() == 2);

    StatementNumber* st = new StatementNumber(2);
    Synonym* syn = new Synonym(Synonym::DesignEntity::VARIABLE, "v");
    ModifiesS m(st, syn);

    Clause *c1 = clauses.front();
    requireTrue(*c1 == m);

    Wildcard* w = new Wildcard();
    Expression* expr = new Expression("x");
    Pattern a(w, expr, "a");

    Clause *c2 = clauses.back();
    requireTrue(*c2 == a);
}
