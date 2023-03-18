#include <utility>

#include "qps/clause/one_arg_clause/OneArgClause.h"
#include "pkb/PKBReader.h"
#include "pkb/PKBWriter.h"
#include "commons/types.h"
#include "catch.hpp"
#include "../TestHelper.h"

TEST_CASE("1. Test IfPattern") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    ENT_SET entset = {"x", "y"};
    STMT_SET ifStatements = {1, 2};
    STMT_ENT_SET ifUses = {std::make_pair(1, "x"), std::make_pair(2, "y")};
    pkbWriterPtr->addEntities(Entity::Variable, entset);
    pkbWriterPtr->addStatements(StmtType::If, ifStatements);
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::IfCondVarUses, ifUses);

    SECTION("Test ifs(_, _, _)") {
        IfPattern ifPattern(std::make_unique<Wildcard>(), "");
        BoolResult& actualResult = dynamic_cast<BoolResult &>(*ifPattern.evaluate(pkbReaderPtr));
        // requireEqual(actualResult.b, true);
    }

    SECTION("Test positive ifs(\"x\", _, _)") {
        IfPattern ifPattern(std::make_unique<Ident>("x"), "");
        BoolResult& actualResult = dynamic_cast<BoolResult &>(*ifPattern.evaluate(pkbReaderPtr));
        // requireEqual(actualResult.b, true);
    }

    SECTION("Test negative ifs(\"z\", _, _)") {
        IfPattern ifPattern(std::make_unique<Ident>("z"), "");
        BoolResult& actualResult = dynamic_cast<BoolResult &>(*ifPattern.evaluate(pkbReaderPtr));
        // requireEqual(actualResult.b, false);
    }

    SECTION("Test ifs(var, _, _)") {
        IfPattern ifPattern(std::make_unique<Synonym>(Synonym::DesignEntity::VARIABLE, "var"), "");
        TableResult expectedResult = TableResult("var", std::unordered_set<std::string>{"x", "y"});
        TableResult& actualResult = dynamic_cast<TableResult &>(*ifPattern.evaluate(pkbReaderPtr));
        // requireTrue(actualResult == expectedResult);
    }
}
