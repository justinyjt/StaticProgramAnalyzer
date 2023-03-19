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

    SECTION("Test ifs(_, _, _)") {  // -> int[]
        IfPattern ifPattern(std::make_unique<Wildcard>(), "ifs");
        std::unique_ptr<Result> actualResult = ifPattern.evaluate(pkbReaderPtr);
        TableResult expectedResult("ifs", ifStatements);
        requireTrue(expectedResult == *actualResult);
    }

    SECTION("Test positive ifs('x', _, _)") {  // -> int[]
        IfPattern ifPattern(std::make_unique<Ident>("x"), "ifs");
        std::unique_ptr<Result> actualResult = ifPattern.evaluate(pkbReaderPtr);
        STMT_SET s{1};
        TableResult expectedResult("ifs", s);
        requireTrue(expectedResult == *actualResult);
    }

    SECTION("Test negative ifs('z', _, _)") {  // -> int[]
        IfPattern ifPattern(std::make_unique<Ident>("z"), "ifs");
        std::unique_ptr<Result> actualResult = ifPattern.evaluate(pkbReaderPtr);
        STMT_SET s{};
        TableResult expectedResult("ifs", s);
        requireTrue(expectedResult == *actualResult);
    }

    SECTION("Test ifs(var, _, _)") {  // -> <str, str>[]
        IfPattern ifPattern(std::make_unique<Synonym>(Synonym::DesignEntity::VARIABLE, "var"), "ifs");
        std::unique_ptr<Result> actualResult = ifPattern.evaluate(pkbReaderPtr);
        TableResult expectedResult("ifs", "var", ifUses);
        requireTrue(expectedResult == *actualResult);
    }
}
