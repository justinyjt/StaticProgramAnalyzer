#include "pkb/PKBReader.h"
#include "pkb/PKBWriter.h"
#include "catch.hpp"
#include "../TestHelper.h"

TEST_CASE("1. Test PKB Read Entities", "[PKB][PKBReader][Entity]") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    ENT_SET setA = {"x", "y"};
    ENT_SET setB = {};
    pkbWriterPtr->addEntities(Entity::Variable, setA);
    pkbWriterPtr->addEntities(Entity::Constant, setB);



    SECTION("Read all variables") {
        ENT_SET expectedResult = {"x", "y"};
        ENT_SET actualResult = pkbReaderPtr->getEntities(Entity::Variable);
        requireTrue(actualResult == expectedResult);
    }

    SECTION("Read all constants") {
        ENT_SET expectedResult = {};
        ENT_SET actualResult = pkbReaderPtr->getEntities(Entity::Constant);
        requireTrue(actualResult == expectedResult);
    }
}

TEST_CASE("2. Read StmtName Relationship table", "[PKB][PKBReader][Relationship]") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    STMT_ENT_SET usesSet = {std::make_pair(2, "a"),  std::make_pair(1, "b"), std::make_pair(2, "b")};
    STMT_ENT_SET modifiesSet = {};
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Uses, usesSet);
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Modifies, modifiesSet);

    SECTION("Read all Uses table") {
        STMT_ENT_SET expectedResult = {std::make_pair(2, "a"),  std::make_pair(1, "b"), std::make_pair(2, "b")};
        STMT_ENT_SET actualResult = pkbReaderPtr->getAllRelationships(StmtNameRelationship::Uses);
        requireTrue(actualResult == expectedResult);
    }

    SECTION("Read all Modifies Table") {
        STMT_ENT_SET expectedResult = {};
        STMT_ENT_SET actualResult = pkbReaderPtr->getAllRelationships(StmtNameRelationship::Modifies);
        requireTrue(actualResult == expectedResult);
    }
}

TEST_CASE("3. Read statements from StmtNameRelationship table", "[PKB][PKBReader][Relationship]") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    STMT_ENT_SET usesSet = {std::make_pair(2, "a"),  std::make_pair(1, "b"), std::make_pair(2, "b")};
    STMT_ENT_SET modifiesSet = {};
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Uses, usesSet);
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Modifies, modifiesSet);

    SECTION("Return empty set for non-existent variable in uses table") {
        STMT_SET expectedResult = {};
        STMT_SET actualResult = pkbReaderPtr->getRelationship(StmtNameRelationship::Uses, "non-existent");
        requireTrue(actualResult == expectedResult);
    }
    
    SECTION("Read statement given variable in uses table") {
        STMT_SET expectedResult = {1, 2};
        STMT_SET actualResult = pkbReaderPtr->getRelationship(StmtNameRelationship::Uses, "b");
        requireTrue(actualResult == expectedResult);
    }

    SECTION("Return empty set for non-existent variable in modifies table") {
        STMT_SET expectedResult = {};
        STMT_SET actualResult = pkbReaderPtr->getRelationship(StmtNameRelationship::Modifies, "non-existent");
        requireTrue(actualResult == expectedResult);
    }

    SECTION("Read statement given variable in modifies table") {
        STMT_SET expectedResult = {};
        STMT_SET actualResult = pkbReaderPtr->getRelationship(StmtNameRelationship::Modifies, "a");
        requireTrue(actualResult == expectedResult);
    }
}

TEST_CASE("4. Read variables from StmtNameRelationship table", "[PKB][PKBReader][Relationship]") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    STMT_ENT_SET usesSet = {std::make_pair(2, "a"),  std::make_pair(1, "b"), std::make_pair(2, "b")};
    STMT_ENT_SET modifiesSet = {};
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Uses, usesSet);
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Modifies, modifiesSet);


    SECTION("Read variable given statement in modifies/uses table") {
        ENT_SET expectedResultForUses = {"a", "b"};
        ENT_SET expectedResultForModifies = {};
        ENT_SET actualResultForUses = pkbReaderPtr->getRelationship(StmtNameRelationship::Uses, 2);
        ENT_SET actualResultForModifies = pkbReaderPtr->getRelationship(StmtNameRelationship::Modifies, 1);
        requireTrue(actualResultForUses == expectedResultForUses);
        requireTrue(actualResultForModifies == actualResultForModifies);
    }

    SECTION("Return empty set for non-existent statement in uses table") {
        ENT_SET expectedResult = {};
        ENT_SET actualResult = pkbReaderPtr->getRelationship(StmtNameRelationship::Uses, 5);
        requireTrue(actualResult == expectedResult);
    }
}


TEST_CASE("5. Test isRelationshipExists") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    STMT_ENT_SET usesSet = {std::make_pair(2, "a"),  std::make_pair(1, "b"), std::make_pair(2, "b")};
    STMT_ENT_SET modifiesSet = {};
    ENT_ENT_SET usesSet1 = {std::make_pair("main", "x"), std::make_pair("main", "y")};
    ENT_ENT_SET modifiesSet1 = {};
    STMT_STMT_SET followsSet = {std::make_pair(2, 3),  std::make_pair(1, 2), std::make_pair(4, 5)};
    STMT_STMT_SET parentSet = {};

    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Uses, usesSet);
    pkbWriterPtr->addStmtEntityRelationships(StmtNameRelationship::Modifies, modifiesSet);
    pkbWriterPtr->addEntityEntityRelationships(NameNameRelationship::Uses, usesSet1);
    pkbWriterPtr->addEntityEntityRelationships(NameNameRelationship::Modifies, modifiesSet1);
    pkbWriterPtr->addStmtStmtRelationships(StmtStmtRelationship::Follows, followsSet);
    pkbWriterPtr->addStmtStmtRelationships(StmtStmtRelationship::Parent, parentSet);


    SECTION("Check if relationship exists given valid arguments in StmtName Table") {
        requireTrue(pkbReaderPtr->isRelationshipExists(StmtNameRelationship::Uses, 1, "b"));
        requireTrue(pkbReaderPtr->isRelationshipExists(StmtNameRelationship::Uses, 1, "a") == false);
    }

    SECTION("Returns false for empty tables") {
        requireTrue(pkbReaderPtr->isRelationshipExists(StmtNameRelationship::Modifies, 4, "a") == false);
    }

    SECTION("Return false for non-existent statement number") {
        requireTrue(pkbReaderPtr->isRelationshipExists(StmtNameRelationship::Modifies, 5, "b") == false);
    }

    SECTION("Return false for non-existent variable") {
        requireTrue(pkbReaderPtr->isRelationshipExists(StmtNameRelationship::Modifies, 2, "a") == false);
    }

    SECTION("Check if relationship exists given valid arguments in NameName Table") {
        requireTrue(pkbReaderPtr->isRelationshipExists(NameNameRelationship::Uses, "main", "x"));
    }

    SECTION("Return false for empty tables") {
        requireTrue(pkbReaderPtr->isRelationshipExists(NameNameRelationship::Modifies, "main", "x") == false);
    }

    SECTION("Return false for non-existent key variable") {
        requireFalse(pkbReaderPtr->isRelationshipExists(NameNameRelationship::Modifies,
                                                   "notHere", "y"));
    }

    SECTION("Return false for non-existent value variable") {
        requireFalse(pkbReaderPtr->isRelationshipExists(NameNameRelationship::Modifies,
                                                   "main", "a"));
    }

    SECTION("Check if relationship exists given valid arguments in StmtStmt Table") {
        requireTrue(pkbReaderPtr->isRelationshipExists(StmtStmtRelationship::Follows, 1, 2));
    }

    SECTION("Return false for empty tables") {
        requireFalse(pkbReaderPtr->isRelationshipExists(StmtStmtRelationship::Parent, 1, 2));
    }

    SECTION("Return false for non-existent key stmt") {
        requireFalse(pkbReaderPtr->isRelationshipExists(StmtStmtRelationship::Follows, 10, 2));
    }

    SECTION("Return false for non-existent value stmt") {
        requireFalse(pkbReaderPtr->isRelationshipExists(StmtStmtRelationship::Follows, 1, 20));
    }
}

TEST_CASE("6. Read relationships in NameName table", "[PKB]") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    ENT_ENT_SET usesSet = {std::make_pair("main", "x"), std::make_pair("main", "y")};
    ENT_ENT_SET modifiesSet = {};
    pkbWriterPtr->addEntityEntityRelationships(NameNameRelationship::Uses, usesSet);
    pkbWriterPtr->addEntityEntityRelationships(NameNameRelationship::Modifies, modifiesSet);


    SECTION("Read all uses relationship") {
        ENT_ENT_SET expectedResult = {std::make_pair("main", "x"), std::make_pair("main", "y")};
        ENT_ENT_SET actualResult = pkbReaderPtr->getAllRelationships(NameNameRelationship::Uses);
        requireTrue(actualResult == expectedResult);
    }

    SECTION("Read all modifies relationship") {
        ENT_ENT_SET expectedResult = {};
        ENT_ENT_SET actualResult = pkbReaderPtr->getAllRelationships(NameNameRelationship::Modifies);
        requireTrue(actualResult == expectedResult);
    }
}

TEST_CASE("7. Read keys in a NameName Relationship table") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    ENT_ENT_SET usesSet = {std::make_pair("main", "x"), std::make_pair("main", "y")};
    ENT_ENT_SET modifiesSet = {};
    pkbWriterPtr->addEntityEntityRelationships(NameNameRelationship::Uses, usesSet);
    pkbWriterPtr->addEntityEntityRelationships(NameNameRelationship::Modifies, modifiesSet);



    SECTION("Read key given variable which exists in uses table") {
        ENT_SET expectedResult = {"main"};
        ENT_SET actualResult = pkbReaderPtr->getRelationshipByVal(NameNameRelationship::Uses, "y");
        requireTrue(actualResult == expectedResult);
    }

    SECTION("Read key given a non-existent variable that does not exist in uses table") {
        ENT_SET expectedResult = {};
        ENT_SET actualResult = pkbReaderPtr->getRelationshipByVal(NameNameRelationship::Uses, "a");
        requireTrue(actualResult == expectedResult);
    }

    SECTION("Read key given variable which exists in modifies table") {
        ENT_SET expectedResult = {};
        ENT_SET actualResult = pkbReaderPtr->getRelationshipByVal(NameNameRelationship::Modifies, "x");
        requireTrue(actualResult == expectedResult);
    }
}TEST_CASE("8. Read values in a NameName Relationship table") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    ENT_ENT_SET usesSet = {std::make_pair("main", "x"), std::make_pair("main", "y")};
    ENT_ENT_SET modifiesSet = {};
    pkbWriterPtr->addEntityEntityRelationships(NameNameRelationship::Uses, usesSet);
    pkbWriterPtr->addEntityEntityRelationships(NameNameRelationship::Modifies, modifiesSet);

    SECTION("Read value using valid key") {
        ENT_SET expectedResultForUses = {"x", "y"};
        ENT_SET expectedResultForModifies = {};
        ENT_SET actualResultForUses = pkbReaderPtr->getRelationshipByKey(NameNameRelationship::Uses,
                                                                         "main");
        ENT_SET actualResultForModifies = pkbReaderPtr->getRelationshipByKey(NameNameRelationship::Modifies,
                                                                             "main");
        requireTrue(actualResultForUses == expectedResultForUses);
        requireTrue(actualResultForModifies == actualResultForModifies);
    }

    SECTION("Return empty set for invalid key") {
        ENT_SET expectedResult = {};
        ENT_SET actualResult = pkbReaderPtr->getRelationshipByKey(NameNameRelationship::Uses, "non-existent");
        requireTrue(actualResult == expectedResult);
    }
}

TEST_CASE("9. Test getRelationshipByVal for statements") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    STMT_STMT_SET followsSet = {std::make_pair(2, 3),  std::make_pair(3, 4), std::make_pair(5, 6)};
    STMT_STMT_SET parentsSet = {std::make_pair(4, 5),  std::make_pair(4, 6)};
    pkbWriterPtr->addStmtStmtRelationships(StmtStmtRelationship::Follows, followsSet);
    pkbWriterPtr->addStmtStmtRelationships(StmtStmtRelationship::Parent, parentsSet);

    SECTION("Read key from a valid value for Follows") {
        STMT_SET expected = {3};
        STMT_SET actual = pkbReaderPtr->getRelationshipByVal(StmtStmtRelationship::Follows, 4);
        requireTrue(actual == expected);
    }

    SECTION("Read key from a valid value for Parent") {
        STMT_SET expected = {};
        STMT_SET actual = pkbReaderPtr->getRelationshipByVal(StmtStmtRelationship::Parent, 2);
        requireTrue(actual == expected);
    }

    SECTION("Read key from an invalid value for Follows") {
        STMT_SET expected = {};
        STMT_SET actual = pkbReaderPtr->getRelationshipByVal(StmtStmtRelationship::Follows, 9);
        requireTrue(actual == expected);
    }

    SECTION("Read key from an invalid value for Parent") {
        STMT_SET expected = {};
        STMT_SET actual = pkbReaderPtr->getRelationshipByVal(StmtStmtRelationship::Parent, 9);
        requireTrue(actual == expected);
    }
}

TEST_CASE("10. Test getRelationshipByKey for statements") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    STMT_STMT_SET followsSet = {std::make_pair(2, 3),  std::make_pair(3, 4), std::make_pair(5, 6)};
    STMT_STMT_SET parentsSet = {std::make_pair(4, 5),  std::make_pair(4, 6)};
    pkbWriterPtr->addStmtStmtRelationships(StmtStmtRelationship::Follows, followsSet);
    pkbWriterPtr->addStmtStmtRelationships(StmtStmtRelationship::Parent, parentsSet);

    SECTION("Read value from a key value for Follows") {
        STMT_SET expected = {6};
        STMT_SET actual = pkbReaderPtr->getRelationshipByKey(StmtStmtRelationship::Follows, 5);
        requireTrue(actual == expected);
    }

    SECTION("Read value from a key value for Follows") {
        STMT_SET expected = {5, 6};
        STMT_SET actual = pkbReaderPtr->getRelationshipByKey(StmtStmtRelationship::Parent, 4);
        requireTrue(actual == expected);
    }

    SECTION("Read value from an invalid key for Follows") {
        STMT_SET expected = {};
        STMT_SET actual = pkbReaderPtr->getRelationshipByKey(StmtStmtRelationship::Follows, 9);
        requireTrue(actual == expected);
    }

    SECTION("Read value from an invalid key for Parent") {
        STMT_SET expected = {};
        STMT_SET actual = pkbReaderPtr->getRelationshipByKey(StmtStmtRelationship::Parent, 9);
        requireTrue(actual == expected);
    }
}

TEST_CASE("11. Test PKB Read Statements") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;
    STMT_SET setA = {1, 2};
    STMT_SET setW = {3, 4};
    STMT_SET setR = {5, 6};
    STMT_SET setI = {7, 8};
    STMT_SET setC = {};
    STMT_SET setP = {};
    STMT_SET setN = {1, 2, 3, 4, 5, 6, 7, 8};

    pkbWriterPtr->addStatements(StmtType::Assign, setA);
    pkbWriterPtr->addStatements(StmtType::While, setW);
    pkbWriterPtr->addStatements(StmtType::Read, setR);
    pkbWriterPtr->addStatements(StmtType::If, setI);
    pkbWriterPtr->addStatements(StmtType::Call, setC);
    pkbWriterPtr->addStatements(StmtType::Print, setP);
    pkbWriterPtr->addStatements(StmtType::None, setN);

    SECTION("Read assign statements") {
        STMT_SET expected = {1, 2};
        STMT_SET actual = pkbReaderPtr->getStatements(StmtType::Assign);
        requireTrue(actual == expected);
    }

    SECTION("Read while statements") {
        STMT_SET expected = {3, 4};
        STMT_SET actual = pkbReaderPtr->getStatements(StmtType::While);
        requireTrue(actual == expected);
    }

    SECTION("Read read statements") {
        STMT_SET expected = {5, 6};
        STMT_SET actual = pkbReaderPtr->getStatements(StmtType::Read);
        requireTrue(actual == expected);
    }

    SECTION("Read if statements") {
        STMT_SET expected = {7, 8};
        STMT_SET actual = pkbReaderPtr->getStatements(StmtType::If);
        requireTrue(actual == expected);
    }

    SECTION("Read print statements") {
        STMT_SET expected = {};
        STMT_SET actual = pkbReaderPtr->getStatements(StmtType::Print);
        requireTrue(actual == expected);
    }

    SECTION("Read none statements") {
        STMT_SET expected = {1, 2, 3, 4, 5, 6, 7, 8};
        STMT_SET actual = pkbReaderPtr->getStatements(StmtType::None);
        requireTrue(actual == expected);
    }
}

TEST_CASE("12. Test PKB Pattern Matching") {
    PKB pkb;
    PKBWriter pkbWriter(pkb);
    PKBReader pkbReader(pkb);
    PKBWriter *pkbWriterPtr = &pkbWriter;
    PKBReader *pkbReaderPtr = &pkbReader;

    std::unordered_map<STMT_NUM, std::string> patternMap = {std::make_pair(1, "y + 1"), std::make_pair(2, "y")};
    pkbWriterPtr->addPatterns(patternMap);

    SECTION("Test Existing Partial Match") {
        STMT_SET expected = {1, 2};
        std::string pattern = "y";
        STMT_SET actual = pkbReaderPtr->getStmtWithPartialPatternMatch(pattern);
        requireTrue(expected == actual);
    }

    SECTION("Test Inexistent Partial Match") {
        STMT_SET expected = {};
        std::string pattern = "inexistent";
        STMT_SET actual = pkbReaderPtr->getStmtWithPartialPatternMatch(pattern);
        requireTrue(expected == actual);
    }

    SECTION("Test Valid Exact Match") {
        STMT_SET expected = {2};
        std::string pattern = "y";
        STMT_SET actual = pkbReaderPtr->getStmtWithExactPatternMatch(pattern);
        requireTrue(expected == actual);
    }

    SECTION("Test Inexistent Exact Match") {
        STMT_SET expected = {};
        std::string pattern = "x";
        STMT_SET actual = pkbReaderPtr->getStmtWithExactPatternMatch(pattern);
        requireTrue(expected == actual);
    }
}
