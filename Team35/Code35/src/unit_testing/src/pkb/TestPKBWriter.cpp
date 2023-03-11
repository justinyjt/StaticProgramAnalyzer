#include <utility>

#include "pkb/PKBWriter.h"
#include "commons/types.h"
#include "catch.hpp"

/* Named Entity adding Tests */

// ConstValue entity adding
TEST_CASE("Test PKB adding a statement", "[pkb][statement]") {
    PKB p;
    PKBWriter pkbWriter(p);
    PKBWriter *pkbWriterPtr = &pkbWriter;

    SECTION("Test Pkb adding a constant statement number") {
        ENT_SET entSetConstant;
        entSetConstant.emplace("1");
        REQUIRE(pkbWriterPtr->addEntities(Entity::Constant, entSetConstant));
    }

    SECTION("Test Pkb adding a constant statement number") {
        ENT_SET entSetVariable;
        entSetVariable.emplace("x");
        REQUIRE(pkbWriterPtr->addEntities(Entity::Variable, entSetVariable));
    }
}

TEST_CASE("Test Pkb adding multiple statements", "[pkb][statement][multiple]") {
    PKB p;
    PKBWriter pkbWriter(p);
    PKBWriter *pkbWriterPtr = &pkbWriter;

    SECTION("Test Pkb adding multiple constant and variable statements") {
        ENT_SET entSetConstant;
        entSetConstant.emplace("1");
        entSetConstant.emplace("2");
        entSetConstant.emplace("3");
        REQUIRE(pkbWriterPtr->addEntities(Entity::Constant, entSetConstant));
        ENT_SET entSetVariable;
        entSetVariable.emplace("x");
        entSetVariable.emplace("y");
        entSetVariable.emplace("z");
        REQUIRE(pkbWriterPtr->addEntities(Entity::Variable, entSetVariable));
    }
}

TEST_CASE("Test Pkb adding a statement, error with repeated stmtNo", "[pkb][statement][error]") {
    PKB p;
    PKBWriter pkbWriter(p);
    PKBWriter *pkbWriterPtr = &pkbWriter;

    SECTION("Test Pkb adding repeated constant statement, with error") {
        ENT_SET entSetConstant;
        entSetConstant.emplace("1");
        REQUIRE(pkbWriterPtr->addEntities(Entity::Constant, entSetConstant));
        REQUIRE(!pkbWriterPtr->addEntities(Entity::Constant, entSetConstant));
    }

    SECTION("Test Pkb adding repeated variable statement, with error") {
        ENT_SET entSetVariable;
        entSetVariable.emplace("x");
        REQUIRE(pkbWriterPtr->addEntities(Entity::Constant, entSetVariable));
        REQUIRE(!pkbWriterPtr->addEntities(Entity::Constant, entSetVariable));
    }
}

TEST_CASE("Test Pkb adding Statement-Name relationship", "[pkb][relationship]") {
    PKB p;
    PKBWriter pkbWriter(p);
    PKBWriter *pkbWriterPtr = &pkbWriter;

    SECTION("Test Pkb adding Statement-Name Uses relationship") {
        STMT_ENT_SET stmtEntSet;
        stmtEntSet.insert(std::make_pair(1, "x"));
        REQUIRE(pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Uses, stmtEntSet));
    }

    SECTION("Test Pkb adding Statement-Name Modifies relationship") {
        STMT_ENT_SET stmtEntSet;
        stmtEntSet.insert(std::make_pair(2, "y"));
        REQUIRE(pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Modifies, stmtEntSet));
    }
}

TEST_CASE("Test Pkb adding Entity-Entity relationship", "[pkb][relationship]") {
    PKB p;
    PKBWriter pkbWriter(p);
    PKBWriter *pkbWriterPtr = &pkbWriter;

    SECTION("Test Pkb adding Entity-Entity Uses relationship") {
        ENT_ENT_SET entEntSet;
        entEntSet.insert(std::make_pair("add", "x"));
        REQUIRE(pkbWriterPtr->addEntityEntityRelationships(NameNameRelationship::Uses, entEntSet));
    }

    SECTION("Test Pkb adding Entity-Entity Modifies relationship") {
        ENT_ENT_SET entEntSet;
        entEntSet.insert(std::make_pair("divide", "y"));
        REQUIRE(pkbWriterPtr->addEntityEntityRelationships(NameNameRelationship::Modifies, entEntSet));
    }

    SECTION("Test Pkb adding Statement-Statement Follows relationship") {
        STMT_STMT_SET stmtStmtSet;
        stmtStmtSet.insert(std::make_pair(1, 2));
        REQUIRE(pkbWriterPtr->addStmtStmtRelationships(StmtStmtRelationship::Follows, stmtStmtSet));
    }

    SECTION("Test Pkb adding Statement-Statement FollowsStar relationship") {
        STMT_STMT_SET stmtStmtSet;
        stmtStmtSet.insert(std::make_pair(1, 2));
        REQUIRE(pkbWriterPtr->addStmtStmtRelationships(StmtStmtRelationship::FollowsStar, stmtStmtSet));
    }

    SECTION("Test Pkb adding Statement-Statement Parent relationship") {
        STMT_STMT_SET stmtStmtSet;
        stmtStmtSet.insert(std::make_pair(1, 2));
        REQUIRE(pkbWriterPtr->addStmtStmtRelationships(StmtStmtRelationship::Parent, stmtStmtSet));
    }

    SECTION("Test Pkb adding Statement-Statement ParentStar relationship") {
        STMT_STMT_SET stmtStmtSet;
        stmtStmtSet.insert(std::make_pair(1, 2));
        REQUIRE(pkbWriterPtr->addStmtStmtRelationships(StmtStmtRelationship::ParentStar, stmtStmtSet));
    }
}

TEST_CASE("Test Pkb adding multiple Statement-Entity relationships", "[pkb][relationship][multiple]") {
    PKB p;
    PKBWriter pkbWriter(p);
    PKBWriter *pkbWriterPtr = &pkbWriter;

    SECTION("Test Pkb adding multiple Statement-Entity Uses relationships") {
        STMT_ENT_SET stmtEntSet;
        stmtEntSet.insert(std::make_pair(1, "x"));
        stmtEntSet.insert(std::make_pair(2, "y"));
        stmtEntSet.insert(std::make_pair(3, "z"));
        REQUIRE(pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Uses, stmtEntSet));
    }
    
        // can use same parameters because adding to different table
    SECTION("Test Pkb adding multiple Statement-Name Modifies relationships") {
        STMT_ENT_SET stmtEntSet;
        stmtEntSet.insert(std::make_pair(1, "x"));
        stmtEntSet.insert(std::make_pair(2, "y"));
        stmtEntSet.insert(std::make_pair(3, "z"));
        REQUIRE(pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Modifies, stmtEntSet));
    }
}

