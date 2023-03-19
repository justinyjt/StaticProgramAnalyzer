#include <utility>

#include "qps/clause/TwoArgClause/WithEntClause.h"
#include "pkb/PKBReader.h"
#include "pkb/PKBWriter.h"
#include "commons/types.h"
#include "catch.hpp"
#include "../TestHelper.h"

TEST_CASE("1. Test WithEntClause") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    ENT_SET setA{"b", "c"};
    ENT_SET setB{"a", "b"};
    STMT_SET printStmts{1, 2};
    STMT_SET readStmts{3, 4};
    STMT_ENT_SET usesRs{std::make_pair(1, "a"), std::make_pair(2, "b")};
    STMT_ENT_SET modifiesRs{std::make_pair(3, "b"), std::make_pair(4, "c")};
    pkbWriterPtr->addEntities(Entity::Variable, setA);
    pkbWriterPtr->addEntities(Entity::Procedure, setB);
    pkbWriterPtr->addStatements(StmtType::Print, printStmts);
    pkbWriterPtr->addStatements(StmtType::Read, readStmts);
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Uses, usesRs);
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Modifies, modifiesRs);
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::PrintStmtVar, usesRs);
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::ReadStmtVar, modifiesRs);

    SECTION("Test with ent = ent") {
        std::unique_ptr<Ident> arg1 = std::make_unique<Ident>("a");
        std::unique_ptr<Ident> arg2 = std::make_unique<Ident>("a");
        WithEntClause withEntClause(std::move(arg1), std::move(arg2));
        auto actualResult = withEntClause.evaluate(pkbReaderPtr);
        requireTrue(BoolResult(true) == *actualResult);
    }

    SECTION("Test with syn = ent") {
        std::unique_ptr<Synonym> arg1 = std::make_unique<Synonym>(Synonym::DesignEntity::PROCEDURE, "p");
        std::unique_ptr<Ident> arg2 = std::make_unique<Ident>("a");
        WithEntClause withEntClause(std::move(arg1), std::move(arg2));
        ENT_SET s{"a"};
        TableResult expectedResult("p", s);
        auto actualResult = withEntClause.evaluate(pkbReaderPtr);
        requireTrue(expectedResult == *actualResult);
    }

    SECTION("Test with syn = ent with print") {
        std::unique_ptr<Synonym> arg1 = std::make_unique<Synonym>(Synonym::DesignEntity::PRINT, "pr");
        std::unique_ptr<Ident> arg2 = std::make_unique<Ident>("a");
        WithEntClause withEntClause = WithEntClause(std::move(arg1), std::move(arg2));
        ENT_SET s{"1"};
        TableResult expectedResult("pr", s);
        auto actualResult = withEntClause.evaluate(pkbReaderPtr);
        requireTrue(expectedResult == *actualResult);
    }

    SECTION("Test with syn = num with read") {
        std::unique_ptr<Synonym> arg1 = std::make_unique<Synonym>(Synonym::DesignEntity::READ, "r");
        std::unique_ptr<Ident> arg2 = std::make_unique<Ident>("c");
        WithEntClause withEntClause = WithEntClause(std::move(arg1), std::move(arg2));
        ENT_SET s{"4"};
        TableResult expectedResult = TableResult("r", s);
        auto actualResult = withEntClause.evaluate(pkbReaderPtr);
        requireTrue(expectedResult == *actualResult);
    }

    SECTION("Test with syn = syn") {
        std::unique_ptr<Synonym> arg1 = std::make_unique<Synonym>(Synonym::DesignEntity::PROCEDURE, "p");
        std::unique_ptr<Synonym> arg2 = std::make_unique<Synonym>(Synonym::DesignEntity::VARIABLE, "v");
        WithEntClause withEntClause = WithEntClause(std::move(arg1), std::move(arg2));
        ENT_ENT_SET s{std::make_pair("b", "b")};
        TableResult expectedResult("p", "v", s);
        auto actualResult = withEntClause.evaluate(pkbReaderPtr);
        requireTrue(expectedResult == *actualResult);
    }

    SECTION("Test with syn = syn print, read") {
        std::unique_ptr<Synonym> arg1 = std::make_unique<Synonym>(Synonym::DesignEntity::PRINT, "pr");
        std::unique_ptr<Synonym> arg2 = std::make_unique<Synonym>(Synonym::DesignEntity::READ, "r");
        WithEntClause withEntClause = WithEntClause(std::move(arg1), std::move(arg2));
        STMT_ENT_SET printSet{STMT_ENT(1, "a"), STMT_ENT(2, "b")};
        STMT_ENT_SET readSet{STMT_ENT(3, "b"), STMT_ENT(4, "c")};
        auto expectedResult = dynamic_cast<TableResult &>(*Result::join(*std::make_unique<TableResult>(
            TableResult("pr", "common-name", printSet)),
           *std::make_unique<TableResult>(TableResult("r", "common-name", readSet))));
        auto actualResult = withEntClause.evaluate(pkbReaderPtr);
        requireTrue(expectedResult == *actualResult);
    }
}
