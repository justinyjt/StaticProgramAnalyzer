#include <utility>

#include "pkb/PKBWriter.h"
#include "commons/types.h"
#include "catch.hpp"
#include "../TestHelper.h"

/* Named Entity adding Tests */

// ConstValue entity adding
TEST_CASE("Test PKB adding a statement", "[pkb][statement]") {
    PKB p;
    PKBWriter pkbWriter(p);
    PKBWriter *pkbWriterPtr = &pkbWriter;

    SECTION("Test PKB adding a constant statement number") {
        ENT_SET entSetConstant;
        entSetConstant.emplace("1");
        pkbWriterPtr->addEntities(Entity::Constant, entSetConstant);
        requireEqual(entSetConstant, p.getEntityTable(Entity::Constant).getTable());
    }

    SECTION("Test PKB adding a constant statement number") {
        ENT_SET entSetVariable;
        entSetVariable.emplace("x");
        pkbWriterPtr->addEntities(Entity::Variable, entSetVariable);
        requireEqual(entSetVariable, p.getEntityTable(Entity::Variable).getTable());
    }
}

TEST_CASE("Test PKB adding multiple statements", "[pkb][statement][multiple]") {
    PKB p;
    PKBWriter pkbWriter(p);
    PKBWriter *pkbWriterPtr = &pkbWriter;

    SECTION("Test PKB adding multiple constant and variable statements") {
        ENT_SET entSetConstant;
        entSetConstant.emplace("1");
        entSetConstant.emplace("2");
        entSetConstant.emplace("3");
        pkbWriterPtr->addEntities(Entity::Constant, entSetConstant);
        requireEqual(entSetConstant, p.getEntityTable(Entity::Constant).getTable());
        ENT_SET entSetVariable;
        entSetVariable.emplace("x");
        entSetVariable.emplace("y");
        entSetVariable.emplace("z");
        pkbWriterPtr->addEntities(Entity::Variable, entSetVariable);
        requireEqual(entSetVariable, p.getEntityTable(Entity::Variable).getTable());
    }
}

TEST_CASE("Test PKB adding a statement, error with repeated stmtNo", "[pkb][statement][error]") {
    PKB p;
    PKBWriter pkbWriter(p);
    PKBWriter *pkbWriterPtr = &pkbWriter;

    SECTION("Test PKB adding repeated constant statement, with no repeat") {
        ENT_SET entSetConstant;
        entSetConstant.emplace("1");
        pkbWriterPtr->addEntities(Entity::Constant, entSetConstant);
        pkbWriterPtr->addEntities(Entity::Constant, entSetConstant);
        requireEqual(entSetConstant, p.getEntityTable(Entity::Constant).getTable());
    }

    SECTION("Test PKB adding repeated variable statement, with no repeat") {
        ENT_SET entSetVariable;
        entSetVariable.emplace("x");
        pkbWriterPtr->addEntities(Entity::Variable, entSetVariable);
        pkbWriterPtr->addEntities(Entity::Variable, entSetVariable);
        requireEqual(entSetVariable, p.getEntityTable(Entity::Variable).getTable());
    }
}

TEST_CASE("Test PKB adding Statement-Name relationship", "[pkb][relationship]") {
    PKB p;
    PKBWriter pkbWriter(p);
    PKBWriter *pkbWriterPtr = &pkbWriter;

    SECTION("Test PKB adding Statement-Name Uses relationship") {
        STMT_ENT_SET stmtEntSet;
        stmtEntSet.insert(std::make_pair(1, "x"));
        pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Uses, stmtEntSet);
        requireEqual(stmtEntSet, p.getStmtNameRelationshipTable(StmtNameRelationship::Uses).getKeyValuePairs());
    }

    SECTION("Test PKB adding Statement-Name Modifies relationship") {
        STMT_ENT_SET stmtEntSet;
        stmtEntSet.insert(std::make_pair(2, "y"));
        pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Modifies, stmtEntSet);
        requireEqual(stmtEntSet, p.getStmtNameRelationshipTable(StmtNameRelationship::Modifies).getKeyValuePairs());
    }
}

TEST_CASE("Test PKB adding Entity-Entity relationship", "[pkb][relationship]") {
    PKB p;
    PKBWriter pkbWriter(p);
    PKBWriter *pkbWriterPtr = &pkbWriter;

    SECTION("Test PKB adding Entity-Entity Uses relationship") {
        ENT_ENT_SET entEntSet;
        entEntSet.insert(std::make_pair("add", "x"));
        pkbWriterPtr->addEntityEntityRelationships(NameNameRelationship::Uses, entEntSet);
        requireEqual(entEntSet, p.getNameNameRelationshipTable(NameNameRelationship::Uses).getKeyValuePairs());
    }

    SECTION("Test PKB adding Entity-Entity Modifies relationship") {
        ENT_ENT_SET entEntSet;
        entEntSet.insert(std::make_pair("divide", "y"));
        pkbWriterPtr->addEntityEntityRelationships(NameNameRelationship::Modifies, entEntSet);
        requireEqual(entEntSet, p.getNameNameRelationshipTable(NameNameRelationship::Modifies).getKeyValuePairs());
    }

    SECTION("Test PKB adding Statement-Statement Follows relationship") {
        STMT_STMT_SET stmtStmtSet;
        stmtStmtSet.insert(std::make_pair(1, 2));
        pkbWriterPtr->addStmtStmtRelationships(StmtStmtRelationship::Follows, stmtStmtSet);
        requireEqual(stmtStmtSet, p.getStmtStmtRelationshipTable(StmtStmtRelationship::Follows).getKeyValuePairs());
    }

    SECTION("Test PKB adding Statement-Statement FollowsStar relationship") {
        STMT_STMT_SET stmtStmtSet;
        stmtStmtSet.insert(std::make_pair(1, 2));
        pkbWriterPtr->addStmtStmtRelationships(StmtStmtRelationship::FollowsStar, stmtStmtSet);
        requireEqual(stmtStmtSet, p.getStmtStmtRelationshipTable(StmtStmtRelationship::FollowsStar).getKeyValuePairs());
    }

    SECTION("Test PKB adding Statement-Statement Parent relationship") {
        STMT_STMT_SET stmtStmtSet;
        stmtStmtSet.insert(std::make_pair(1, 2));
        pkbWriterPtr->addStmtStmtRelationships(StmtStmtRelationship::Parent, stmtStmtSet);
        requireEqual(stmtStmtSet, p.getStmtStmtRelationshipTable(StmtStmtRelationship::Parent).getKeyValuePairs());
    }

    SECTION("Test PKB adding Statement-Statement ParentStar relationship") {
        STMT_STMT_SET stmtStmtSet;
        stmtStmtSet.insert(std::make_pair(1, 2));
        pkbWriterPtr->addStmtStmtRelationships(StmtStmtRelationship::ParentStar, stmtStmtSet);
        requireEqual(stmtStmtSet, p.getStmtStmtRelationshipTable(StmtStmtRelationship::ParentStar).getKeyValuePairs());
    }
}

TEST_CASE("Test PKB adding multiple Statement-Entity relationships", "[pkb][relationship][multiple]") {
    PKB p;
    PKBWriter pkbWriter(p);
    PKBWriter *pkbWriterPtr = &pkbWriter;

    SECTION("Test PKB adding multiple Statement-Entity Uses relationships") {
        STMT_ENT_SET stmtEntSet;
        stmtEntSet.insert(std::make_pair(1, "x"));
        stmtEntSet.insert(std::make_pair(2, "y"));
        stmtEntSet.insert(std::make_pair(3, "z"));
        pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Uses, stmtEntSet);
        requireEqual(stmtEntSet, p.getStmtNameRelationshipTable(StmtNameRelationship::Uses).getKeyValuePairs());
    }

        // can use same parameters because adding to different table
    SECTION("Test PKB adding multiple Statement-Name Modifies relationships") {
        STMT_ENT_SET stmtEntSet;
        stmtEntSet.insert(std::make_pair(1, "x"));
        stmtEntSet.insert(std::make_pair(2, "y"));
        stmtEntSet.insert(std::make_pair(3, "z"));
        pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Modifies, stmtEntSet);
        requireEqual(stmtEntSet, p.getStmtNameRelationshipTable(StmtNameRelationship::Modifies).getKeyValuePairs());
    }
}

