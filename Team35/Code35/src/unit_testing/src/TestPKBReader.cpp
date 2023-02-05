#include "pkb/PKBReader.h"
#include "pkb/PKBWriter.h"
#include "catch.hpp"


TEST_CASE("PKB can get all named entities", "[PKB]") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    ENT_SET setA = {"x", "y"};
    ENT_SET setB = {};
    pkbWriterPtr->addEntities(Entity::Variable, setA);
    pkbWriterPtr->addEntities(Entity::Constant, setB);



    SECTION("PKB can get all variables") {
        ENT_SET expectedResult = {"x", "y"};
        ENT_SET actualResult = pkbReaderPtr->getEntities(Entity::Variable);
        REQUIRE(actualResult == expectedResult);
    }

//    SECTION("PKB can get all procedures") {
//        ENT_SET expectedResult = {"main"};
//        ENT_SET actualResult = pkbReaderPtr->getEntities(Entity::Procedure);
//        REQUIRE(actualResult == expectedResult);
//    }

    SECTION("PKB can get all constants") {
        ENT_SET expectedResult = {};
        ENT_SET actualResult = pkbReaderPtr->getEntities(Entity::Constant);
        REQUIRE(actualResult == expectedResult);
    }
}

TEST_CASE("PKB can get all relationships in a Statement-Name table", "[PKB]") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    STMT_ENT_SET usesSet = {std::make_pair(4, "x"),  std::make_pair(4, "y"), std::make_pair(5, "y")};
    STMT_ENT_SET modifiesSet = {};
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Uses, usesSet);
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Modifies, modifiesSet);

    SECTION("PKB can get all uses") {
        STMT_ENT_SET expectedResult = {std::make_pair(4, "x"), std::make_pair(4, "y"), std::make_pair(5, "y")};
        STMT_ENT_SET actualResult = pkbReaderPtr->getAllRelationships(StmtNameRelationship::Uses);
        REQUIRE(actualResult == expectedResult);
    }

    SECTION("PKB can get all modifies") {
        STMT_ENT_SET expectedResult = {};
        STMT_ENT_SET actualResult = pkbReaderPtr->getAllRelationships(StmtNameRelationship::Modifies);
        REQUIRE(actualResult == expectedResult);
    }
}

TEST_CASE("PKB can get statements in a Statement-Name StmtNameRelation") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    STMT_ENT_SET usesSet = {std::make_pair(4, "x"),  std::make_pair(4, "y"), std::make_pair(5, "y")};
    STMT_ENT_SET modifiesSet = {};
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Uses, usesSet);
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Modifies, modifiesSet);
    SECTION("PKB can get statements that use a given variable which exists") {
        STMT_SET expectedResult = {4, 5};
        STMT_SET actualResult = pkbReaderPtr->getRelationship(StmtNameRelationship::Uses, "y");
        REQUIRE(actualResult == expectedResult);
    }

    SECTION("PKB can get statements that uses a variable that does not exist") {
        STMT_SET expectedResult = {};
        STMT_SET actualResult = pkbReaderPtr->getRelationship(StmtNameRelationship::Uses, "blah");
        REQUIRE(actualResult == expectedResult);
    }

    SECTION("PKB can get statements that modifies a variable that exists") {
        STMT_SET expectedResult = {};
        STMT_SET actualResult = pkbReaderPtr->getRelationship(StmtNameRelationship::Modifies, "x");
        REQUIRE(actualResult == expectedResult);
    }
}

TEST_CASE("PKB can get variables in a Statement-Name StmtNameRelation") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    STMT_ENT_SET usesSet = {std::make_pair(4, "x"),  std::make_pair(4, "y"), std::make_pair(5, "y")};
    STMT_ENT_SET modifiesSet = {};
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Uses, usesSet);
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Modifies, modifiesSet);


    SECTION("PKB can get variables that are used/modified by a statement that exists") {
        ENT_SET expectedResultForUses = {"x", "y"};
        ENT_SET expectedResultForModifies = {};
        ENT_SET actualResultForUses = pkbReaderPtr->getRelationship(StmtNameRelationship::Uses, 4);
        ENT_SET actualResultForModifies = pkbReaderPtr->getRelationship(StmtNameRelationship::Modifies, 3);
        REQUIRE(actualResultForUses == expectedResultForUses);
        REQUIRE(actualResultForModifies == actualResultForModifies);
    }

    SECTION("PKB can return empty set for a statement that does not use any variable") {
        ENT_SET expectedResult = {};
        ENT_SET actualResult = pkbReaderPtr->getRelationship(StmtNameRelationship::Uses, 1);
        REQUIRE(actualResult == expectedResult);
    }
}


TEST_CASE("PKB can check if a relationship exists in a table") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    STMT_ENT_SET usesSet = {std::make_pair(4, "x"),  std::make_pair(4, "y"), std::make_pair(5, "y")};
    STMT_ENT_SET modifiesSet = {};
    ENT_ENT_SET usesSet1 = {std::make_pair("main", "x"), std::make_pair("main", "y")};
    ENT_ENT_SET modifiesSet1 = {};
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Uses, usesSet);
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Modifies, modifiesSet);
    pkbWriterPtr->addEntityEntityRelationships(NameNameRelationship::Uses, usesSet1);
    pkbWriterPtr->addEntityEntityRelationships(NameNameRelationship::Modifies, modifiesSet1);

    SECTION("PKB can check if a relationship exists in a Stmt-Name Table with valid params") {
        REQUIRE(pkbReaderPtr->isRelationshipExists(StmtNameRelationship::Uses, 4, "x"));
        REQUIRE(pkbReaderPtr->isRelationshipExists(StmtNameRelationship::Uses, 5, "x") == false);
    }

    SECTION("PKB can return false if Stmt-Name Table is empty") {
        REQUIRE(pkbReaderPtr->isRelationshipExists(StmtNameRelationship::Modifies, 3, "x") == false);
    }

    SECTION("PKB can return false if statement number does not exist in Stmt-Name Table") {
        REQUIRE(pkbReaderPtr->isRelationshipExists(StmtNameRelationship::Modifies, 10, "y") == false);
    }

    SECTION("PKB can return false if variable name does not exist in Stmt-Name Table") {
        REQUIRE(pkbReaderPtr->isRelationshipExists(StmtNameRelationship::Modifies, 2, "a") == false);
    }

    SECTION("PKB can check if a relationship exists in a Name-Name Table with valid params") {
        REQUIRE(pkbReaderPtr->isRelationshipExists(NameNameRelationship::Uses, "main", "x"));
    }SECTION("PKB can return false if Name-Name Table is empty") {
        REQUIRE(pkbReaderPtr->isRelationshipExists(NameNameRelationship::Modifies, "main", "x") == false);
    }

    SECTION("PKB can return false if procedure does not exist") {
        REQUIRE(pkbReaderPtr->isRelationshipExists(NameNameRelationship::Modifies,
                                                   "notHere", "y") == false);
    }

    SECTION("PKB can return false if variable name does not exist") {
        REQUIRE(pkbReaderPtr->isRelationshipExists(NameNameRelationship::Modifies,
                                                   "main", "a") == false);
    }
}

TEST_CASE("PKB can get all relationships in a Name-Name table", "[PKB]") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    STMT_ENT_SET usesSet = {std::make_pair(4, "x"),  std::make_pair(4, "y"), std::make_pair(5, "y")};
    STMT_ENT_SET modifiesSet = {};
    ENT_ENT_SET usesSet1 = {std::make_pair("main", "x"), std::make_pair("main", "y")};
    ENT_ENT_SET modifiesSet1 = {};
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Uses, usesSet);
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Modifies, modifiesSet);
    pkbWriterPtr->addEntityEntityRelationships(NameNameRelationship::Uses, usesSet1);
    pkbWriterPtr->addEntityEntityRelationships(NameNameRelationship::Modifies, modifiesSet1);


    SECTION("PKB can get all uses") {
        ENT_ENT_SET expectedResult = {std::make_pair("main", "x"), std::make_pair("main", "y")};
        ENT_ENT_SET actualResult = pkbReaderPtr->getAllRelationships(NameNameRelationship::Uses);
        REQUIRE(actualResult == expectedResult);
    }

    SECTION("PKB can get all modifies") {
        ENT_ENT_SET expectedResult = {};
        ENT_ENT_SET actualResult = pkbReaderPtr->getAllRelationships(NameNameRelationship::Modifies);
        REQUIRE(actualResult == expectedResult);
    }
}

TEST_CASE("PKB can get keyNames in a Name-Name StmtNameRelation") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    STMT_ENT_SET usesSet = {std::make_pair(4, "x"),  std::make_pair(4, "y"), std::make_pair(5, "y")};
    STMT_ENT_SET modifiesSet = {};
    ENT_ENT_SET usesSet1 = {std::make_pair("main", "x"), std::make_pair("main", "y")};
    ENT_ENT_SET modifiesSet1 = {};
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Uses, usesSet);
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Modifies, modifiesSet);
    pkbWriterPtr->addEntityEntityRelationships(NameNameRelationship::Uses, usesSet1);
    pkbWriterPtr->addEntityEntityRelationships(NameNameRelationship::Modifies, modifiesSet1);



    SECTION("PKB can get keyNames that use a given variable which exists") {
        ENT_SET expectedResult = {"main"};
        ENT_SET actualResult = pkbReaderPtr->getRelationshipByVal(NameNameRelationship::Uses, "y");
        REQUIRE(actualResult == expectedResult);
    }

    SECTION("PKB can get keyNames that uses a variable that does not exist") {
        ENT_SET expectedResult = {};
        ENT_SET actualResult = pkbReaderPtr->getRelationshipByVal(NameNameRelationship::Uses, "a");
        REQUIRE(actualResult == expectedResult);
    }

    SECTION("PKB can get keyNames that modifies a variable that exists") {
        ENT_SET expectedResult = {};
        ENT_SET actualResult = pkbReaderPtr->getRelationshipByVal(NameNameRelationship::Modifies, "x");
        REQUIRE(actualResult == expectedResult);
    }
}TEST_CASE("PKB can get valueName in a Name-Name StmtNameRelation") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    STMT_ENT_SET usesSet = {std::make_pair(4, "x"),  std::make_pair(4, "y"), std::make_pair(5, "y")};
    STMT_ENT_SET modifiesSet = {};
    ENT_ENT_SET usesSet1 = {std::make_pair("main", "x"), std::make_pair("main", "y")};
    ENT_ENT_SET modifiesSet1 = {};
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Uses, usesSet);
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Modifies, modifiesSet);
    pkbWriterPtr->addEntityEntityRelationships(NameNameRelationship::Uses, usesSet1);
    pkbWriterPtr->addEntityEntityRelationships(NameNameRelationship::Modifies, modifiesSet1);



    SECTION("PKB can get valueNames that are used/modified by a keyName that exists") {
        ENT_SET expectedResultForUses = {"x", "y"};
        ENT_SET expectedResultForModifies = {};
        ENT_SET actualResultForUses = pkbReaderPtr->getRelationshipByKey(NameNameRelationship::Uses,
                                                                         "main");
        ENT_SET actualResultForModifies = pkbReaderPtr->getRelationshipByKey(NameNameRelationship::Modifies,
                                                                             "main");
        REQUIRE(actualResultForUses == expectedResultForUses);
        REQUIRE(actualResultForModifies == actualResultForModifies);
    }

    SECTION("PKB can return empty set for an non-existent keyName") {
        ENT_SET expectedResult = {};
        ENT_SET actualResult = pkbReaderPtr->getRelationshipByKey(NameNameRelationship::Uses, "fake");
        REQUIRE(actualResult == expectedResult);
    }
}
