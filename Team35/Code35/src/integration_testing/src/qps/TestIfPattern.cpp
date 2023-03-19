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
    ENT_SET entset{"x", "y"};
    STMT_SET ifStatements{1, 2};
    STMT_ENT_SET ifUses{std::make_pair(1, "x"), std::make_pair(2, "y")};
    pkbWriterPtr->addEntities(Entity::Variable, entset);
    pkbWriterPtr->addStatements(StmtType::If, ifStatements);
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::IfCondVarUses, ifUses);

    SECTION("Test ifs(_, _, _)") {
        IfPattern ifPattern(std::make_unique<Wildcard>(), "");
        std::unique_ptr<Result> actualResult = ifPattern.evaluate(pkbReaderPtr);
        requireTrue(BoolResult(true) == *actualResult);
    }

    SECTION("Test positive ifs(\"x\", _, _)") {
        IfPattern ifPattern(std::make_unique<Ident>("x"), "");
        std::unique_ptr<Result> actualResult = ifPattern.evaluate(pkbReaderPtr);
        requireTrue(BoolResult(true) == *actualResult);
    }

    SECTION("Test negative ifs(\"z\", _, _)") {
        IfPattern ifPattern(std::make_unique<Ident>("z"), "");
        std::unique_ptr<Result> actualResult = ifPattern.evaluate(pkbReaderPtr);
        requireTrue(BoolResult(false) == *actualResult);
    }

    SECTION("Test ifs(var, _, _)") {
        IfPattern ifPattern(std::make_unique<Synonym>(Synonym::DesignEntity::VARIABLE, "var"), "");
        ENT_SET s{"x" , "y"};
        TableResult expectedResult("var", s);
        std::unique_ptr<Result> actualResult = ifPattern.evaluate(pkbReaderPtr);
        requireTrue(expectedResult == *actualResult);
    }
}
