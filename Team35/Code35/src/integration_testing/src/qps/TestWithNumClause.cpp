#include <utility>

#include "qps/clause/two_arg_clause/WithNumClause.h"
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
    STMT_SET setA{1, 2, 3};
    STMT_SET setB{2, 3};
    ENT_SET setC{"1", "2"};
    pkbWriterPtr->addEntities(Entity::Constant, setC);
    pkbWriterPtr->addStatements(StmtType::None, setA);
    pkbWriterPtr->addStatements(StmtType::Assign, setB);


    SECTION("Test with num = num") {
        std::unique_ptr<PQLNumber> arg1 = std::make_unique<PQLNumber>("2");
        std::unique_ptr<PQLNumber> arg2 = std::make_unique<PQLNumber>("2");
        WithNumClause withNumClause = WithNumClause(std::move(arg1), std::move(arg2));
        auto actualResult = withNumClause.evaluate(pkbReaderPtr);
        requireTrue(BoolResult(true) == *actualResult);
    }

    SECTION("Test with syn = num") {
        std::unique_ptr<Synonym> arg1 = std::make_unique<Synonym>(Synonym::DesignEntity::ASSIGN, "a");
        std::unique_ptr<PQLNumber> arg2 = std::make_unique<PQLNumber>("2");
        WithNumClause withNumClause = WithNumClause(std::move(arg1), std::move(arg2));
        STMT_SET s{2};
        TableResult expectedResult("a", s);
        auto actualResult = withNumClause.evaluate(pkbReaderPtr);
        requireTrue(expectedResult == *actualResult);
    }

    SECTION("Test with const syn = num") {
        std::unique_ptr<Synonym> arg1 = std::make_unique<Synonym>(Synonym::DesignEntity::CONSTANT, "c");
        std::unique_ptr<PQLNumber> arg2 = std::make_unique<PQLNumber>("2");
        WithNumClause withNumClause = WithNumClause(std::move(arg1), std::move(arg2));
        STMT_SET s{2};
        TableResult expectedResult("c", s);
        auto actualResult = withNumClause.evaluate(pkbReaderPtr);
        requireTrue(expectedResult == *actualResult);
    }

    SECTION("Test with num = const syn") {
        std::unique_ptr<Synonym> arg2 = std::make_unique<Synonym>(Synonym::DesignEntity::CONSTANT, "c");
        std::unique_ptr<PQLNumber> arg1 = std::make_unique<PQLNumber>("2");
        WithNumClause withNumClause = WithNumClause(std::move(arg1), std::move(arg2));
        STMT_SET s{2};
        TableResult expectedResult("c", s);
        auto actualResult = withNumClause.evaluate(pkbReaderPtr);
        requireTrue(expectedResult == *actualResult);
    }

    SECTION("Test with const syn = syn") {
        std::unique_ptr<Synonym> arg1 = std::make_unique<Synonym>(Synonym::DesignEntity::CONSTANT, "c");
        std::unique_ptr<Synonym> arg2 = std::make_unique<Synonym>(Synonym::DesignEntity::STMT, "s");
        WithNumClause withNumClause(std::move(arg1), std::move(arg2));
        STMT_STMT_SET s{std::make_pair(1, 1), std::make_pair(2, 2)};
        TableResult expectedResult("c", "s", s);
        auto actualResult = withNumClause.evaluate(pkbReaderPtr);
        requireTrue(expectedResult == *actualResult);
    }

    SECTION("Test with syn = const syn") {
        std::unique_ptr<Synonym> arg2 = std::make_unique<Synonym>(Synonym::DesignEntity::CONSTANT, "c");
        std::unique_ptr<Synonym> arg1 = std::make_unique<Synonym>(Synonym::DesignEntity::STMT, "s");
        WithNumClause withNumClause(std::move(arg1), std::move(arg2));
        STMT_STMT_SET s{std::make_pair(1, 1), std::make_pair(2, 2)};
        TableResult expectedResult("c", "s", s);
        auto actualResult = withNumClause.evaluate(pkbReaderPtr);
        requireTrue(expectedResult == *actualResult);
    }

    SECTION("Test with same const syn") {
        std::unique_ptr<Synonym> arg1 = std::make_unique<Synonym>(Synonym::DesignEntity::CONSTANT, "c1");
        std::unique_ptr<Synonym> arg2 = std::make_unique<Synonym>(Synonym::DesignEntity::CONSTANT, "c2");
        WithNumClause withNumClause(std::move(arg1), std::move(arg2));
        STMT_STMT_SET s{std::make_pair(1, 1), std::make_pair(2, 2)};
        TableResult expectedResult("c1", "c2", s);
        auto actualResult = withNumClause.evaluate(pkbReaderPtr);
        requireTrue(expectedResult == *actualResult);
    }

    SECTION("Test with same non const syn") {
        std::unique_ptr<Synonym> arg1 = std::make_unique<Synonym>(Synonym::DesignEntity::ASSIGN, "a");
        std::unique_ptr<Synonym> arg2 = std::make_unique<Synonym>(Synonym::DesignEntity::ASSIGN, "a");
        WithNumClause withNumClause(std::move(arg1), std::move(arg2));
        STMT_SET s{2, 3};
        TableResult expectedResult("a", s);
        auto actualResult = withNumClause.evaluate(pkbReaderPtr);
        requireTrue(expectedResult == *actualResult);
    }

    SECTION("Test with const syn = const syn") {
        std::unique_ptr<Synonym> arg1 = std::make_unique<Synonym>(Synonym::DesignEntity::CONSTANT, "c");
        std::unique_ptr<Synonym> arg2 = std::make_unique<Synonym>(Synonym::DesignEntity::CONSTANT, "c");
        WithNumClause withNumClause(std::move(arg1), std::move(arg2));
        STMT_SET s{1, 2};
        TableResult expectedResult("c", s);
        auto actualResult = withNumClause.evaluate(pkbReaderPtr);
        requireTrue(expectedResult == *actualResult);
    }

    SECTION("Test with syn = syn") {
        std::unique_ptr<Synonym> arg1 = std::make_unique<Synonym>(Synonym::DesignEntity::ASSIGN, "a");
        std::unique_ptr<Synonym> arg2 = std::make_unique<Synonym>(Synonym::DesignEntity::STMT, "s");
        WithNumClause withNumClause(std::move(arg1), std::move(arg2));
        STMT_STMT_SET s{std::make_pair(3, 3), std::make_pair(2, 2)};
        TableResult expectedResult("a", "s", s);
        auto actualResult = withNumClause.evaluate(pkbReaderPtr);
        requireTrue(expectedResult == *actualResult);
    }
}
