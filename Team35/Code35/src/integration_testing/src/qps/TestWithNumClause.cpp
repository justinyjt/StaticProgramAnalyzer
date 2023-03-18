#include <utility>

#include "qps/clause/TwoArgClause/WithNumClause.h"
#include "pkb/PKBReader.h"
#include "pkb/PKBWriter.h"
#include "commons/types.h"
#include "catch.hpp"
#include "../TestHelper.h"

TEST_CASE("1. Test WithNumClause") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    STMT_SET setA = {1, 3};
    STMT_SET setB = {2, 3};
    pkbWriterPtr->addStatements(StmtType::None, setA);
    pkbWriterPtr->addStatements(StmtType::Assign, setB);


    SECTION("Test with num = num") {
        std::unique_ptr<PQLNumber> arg1 = std::make_unique<PQLNumber>(2);
        std::unique_ptr<PQLNumber> arg2 = std::make_unique<PQLNumber>(2);
        WithNumClause withNumClause = WithNumClause(std::move(arg1), std::move(arg2));
        bool expectedResult = true;
        BoolResult actualResult = dynamic_cast<BoolResult &>(*withNumClause.evaluate(pkbReaderPtr));
        requireEqual(actualResult.b, expectedResult);
    }

    SECTION("Test with syn = num") {
        std::unique_ptr<Synonym> arg1 = std::make_unique<Synonym>(Synonym::DesignEntity::ASSIGN, "a");
        std::unique_ptr<PQLNumber> arg2 = std::make_unique<PQLNumber>(2);
        WithNumClause withNumClause = WithNumClause(std::move(arg1), std::move(arg2));
        STMT_SET s = {2};
        TableResult expectedResult = TableResult("a", s);
        TableResult actualResult = dynamic_cast<TableResult &>(*withNumClause.evaluate(pkbReaderPtr));
        requireEqual(actualResult.idents, expectedResult.idents);
        requireEqual(actualResult.rows, expectedResult.rows);
    }

    SECTION("Test with syn = syn") {
        std::unique_ptr<Synonym> arg1 = std::make_unique<Synonym>(Synonym::DesignEntity::ASSIGN, "a");
        std::unique_ptr<Synonym> arg2 = std::make_unique<Synonym>(Synonym::DesignEntity::STMT, "s");
        WithNumClause withNumClause = WithNumClause(std::move(arg1), std::move(arg2));
        STMT_STMT_SET s = {std::make_pair(3, 3)};
        TableResult expectedResult = TableResult("a", "s", s);
        TableResult actualResult = dynamic_cast<TableResult &>(*withNumClause.evaluate(pkbReaderPtr));
        requireEqual(actualResult.idents, expectedResult.idents);
        requireEqual(actualResult.rows, expectedResult.rows);
    }
}
