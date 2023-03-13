#include "catch.hpp"
#include "../../TestHelper.h"
#include "pkb/db/CFGManager.h"
#include "commons/graph/CFGraph.h"

TEST_CASE("1. Test storage and retrieval in CFGManager") {
    CFG::CFGManager manager;
    CFG::CFGraph def;
    CFG::CFGraph one(def, 1, 3, "one");
    CFG::CFGraph two(def, 4, 7, "two");
    CFG::CFGraph three(def, 8, 15, "three");
    CFG::CFGraph four(def, 16, 16, "four");
    CFG::CFGraph test_one(def, 1, 3, "one");
    CFG::CFGraph test_two(def, 4, 7, "two");
    CFG::CFGraph test_three(def, 8, 15, "three");
    CFG::CFGraph test_four(def, 16, 16, "four");
    std::vector<CFG::CFGraph> test;
    test.emplace_back(std::move(one));
    test.emplace_back(std::move(two));
    test.emplace_back(std::move(three));
    test.emplace_back(std::move(four));
    manager.setGraphs(std::move(test));
    SECTION("Basic retrieval of procedure names") {
        requireEqual(manager.getProcName(1), ENT_NAME("one"));
        requireEqual(manager.getProcName(2), ENT_NAME("one"));
        requireEqual(manager.getProcName(3), ENT_NAME("one"));
        requireEqual(manager.getProcName(4), ENT_NAME("two"));
        requireEqual(manager.getProcName(5), ENT_NAME("two"));
        requireEqual(manager.getProcName(6), ENT_NAME("two"));
        requireEqual(manager.getProcName(7), ENT_NAME("two"));
        requireEqual(manager.getProcName(8), ENT_NAME("three"));
        requireEqual(manager.getProcName(9), ENT_NAME("three"));
        requireEqual(manager.getProcName(10), ENT_NAME("three"));
        requireEqual(manager.getProcName(11), ENT_NAME("three"));
        requireEqual(manager.getProcName(12), ENT_NAME("three"));
        requireEqual(manager.getProcName(13), ENT_NAME("three"));
        requireEqual(manager.getProcName(14), ENT_NAME("three"));
        requireEqual(manager.getProcName(15), ENT_NAME("three"));
        requireEqual(manager.getProcName(16), ENT_NAME("four"));
    }

    SECTION("Basic retrieval of cfg") {
        requireEqualRef(manager.getCFG(1), test_one);
        requireEqualRef(manager.getCFG(2), test_one);
        requireEqualRef(manager.getCFG(3), test_one);
        requireEqualRef(manager.getCFG(4), test_two);
        requireEqualRef(manager.getCFG(5), test_two);
        requireEqualRef(manager.getCFG(6), test_two);
        requireEqualRef(manager.getCFG(7), test_two);
        requireEqualRef(manager.getCFG(8), test_three);
        requireEqualRef(manager.getCFG(9), test_three);
        requireEqualRef(manager.getCFG(10), test_three);
        requireEqualRef(manager.getCFG(11), test_three);
        requireEqualRef(manager.getCFG(12), test_three);
        requireEqualRef(manager.getCFG(13), test_three);
        requireEqualRef(manager.getCFG(14), test_three);
        requireEqualRef(manager.getCFG(15), test_three);
        requireEqualRef(manager.getCFG(16), test_four);
    }
}

TEST_CASE("2. Test storage and retrieval in CFGManager") {
    CFG::CFGManager manager;
    CFG::CFGraph def;
    CFG::CFGraph one(def, 1, 3, "one");
    CFG::CFGraph two(def, 4, 4, "two");
    CFG::CFGraph three(def, 5, 10, "three");
    CFG::CFGraph test_one(def, 1, 3, "one");
    CFG::CFGraph test_two(def, 4, 4, "two");
    CFG::CFGraph test_three(def, 5, 10, "three");
    std::vector<CFG::CFGraph> test;
    test.emplace_back(std::move(one));
    test.emplace_back(std::move(two));
    test.emplace_back(std::move(three));
    manager.setGraphs(std::move(test));
    SECTION("Retrieval of procedure names (edge case in middle)") {
        requireEqual(manager.getProcName(1), ENT_NAME("one"));
        requireEqual(manager.getProcName(2), ENT_NAME("one"));
        requireEqual(manager.getProcName(3), ENT_NAME("one"));
        requireEqual(manager.getProcName(4), ENT_NAME("two"));
        requireEqual(manager.getProcName(5), ENT_NAME("three"));
        requireEqual(manager.getProcName(6), ENT_NAME("three"));
        requireEqual(manager.getProcName(7), ENT_NAME("three"));
        requireEqual(manager.getProcName(8), ENT_NAME("three"));
        requireEqual(manager.getProcName(9), ENT_NAME("three"));
        requireEqual(manager.getProcName(10), ENT_NAME("three"));
    }

    SECTION("Retrieval of cfg (edge case in middle)") {
        requireEqualRef(manager.getCFG(1), test_one);
        requireEqualRef(manager.getCFG(2), test_one);
        requireEqualRef(manager.getCFG(3), test_one);
        requireEqualRef(manager.getCFG(4), test_two);
        requireEqualRef(manager.getCFG(5), test_three);
        requireEqualRef(manager.getCFG(6), test_three);
        requireEqualRef(manager.getCFG(7), test_three);
        requireEqualRef(manager.getCFG(8), test_three);
        requireEqualRef(manager.getCFG(9), test_three);
        requireEqualRef(manager.getCFG(10), test_three);
    }
}

TEST_CASE("3. Test storage and retrieval in CFGManager") {
    CFG::CFGManager manager;
    CFG::CFGraph def;
    CFG::CFGraph one(def, 1, 4, "one");
    CFG::CFGraph two(def, 5, 5, "two");
    CFG::CFGraph three(def, 6, 10, "three");
    CFG::CFGraph test_one(def, 1, 4, "one");
    CFG::CFGraph test_two(def, 5, 5, "two");
    CFG::CFGraph test_three(def, 6, 10, "three");
    std::vector<CFG::CFGraph> test;
    test.emplace_back(std::move(one));
    test.emplace_back(std::move(two));
    test.emplace_back(std::move(three));
    manager.setGraphs(std::move(test));
    SECTION("Retrieval of procedure names (edge case in middle)") {
        requireEqual(manager.getProcName(1), ENT_NAME("one"));
        requireEqual(manager.getProcName(2), ENT_NAME("one"));
        requireEqual(manager.getProcName(3), ENT_NAME("one"));
        requireEqual(manager.getProcName(4), ENT_NAME("one"));
        requireEqual(manager.getProcName(5), ENT_NAME("two"));
        requireEqual(manager.getProcName(6), ENT_NAME("three"));
        requireEqual(manager.getProcName(7), ENT_NAME("three"));
        requireEqual(manager.getProcName(8), ENT_NAME("three"));
        requireEqual(manager.getProcName(9), ENT_NAME("three"));
        requireEqual(manager.getProcName(10), ENT_NAME("three"));
    }

    SECTION("Retrieval of cfg (edge case in middle)") {
        requireEqualRef(manager.getCFG(1), test_one);
        requireEqualRef(manager.getCFG(2), test_one);
        requireEqualRef(manager.getCFG(3), test_one);
        requireEqualRef(manager.getCFG(4), test_one);
        requireEqualRef(manager.getCFG(5), test_two);
        requireEqualRef(manager.getCFG(6), test_three);
        requireEqualRef(manager.getCFG(7), test_three);
        requireEqualRef(manager.getCFG(8), test_three);
        requireEqualRef(manager.getCFG(9), test_three);
        requireEqualRef(manager.getCFG(10), test_three);
    }
}

TEST_CASE("4. Test storage and retrieval in CFGManager") {
    CFG::CFGManager manager;
    CFG::CFGraph def;
    CFG::CFGraph one(def, 1, 2, "one");
    CFG::CFGraph two(def, 3, 3, "two");
    CFG::CFGraph three(def, 4, 4, "three");
    CFG::CFGraph four(def, 5, 6, "four");
    CFG::CFGraph test_one(def, 1, 2, "one");
    CFG::CFGraph test_two(def, 3, 3, "two");
    CFG::CFGraph test_three(def, 4, 4, "three");
    CFG::CFGraph test_four(def, 5, 6, "four");
    std::vector<CFG::CFGraph> test;
    test.emplace_back(std::move(one));
    test.emplace_back(std::move(two));
    test.emplace_back(std::move(three));
    test.emplace_back(std::move(four));
    manager.setGraphs(std::move(test));
    SECTION("Retrieval of procedure names (edge case in middle)") {
        requireEqual(manager.getProcName(1), ENT_NAME("one"));
        requireEqual(manager.getProcName(2), ENT_NAME("one"));
        requireEqual(manager.getProcName(3), ENT_NAME("two"));
        requireEqual(manager.getProcName(4), ENT_NAME("three"));
        requireEqual(manager.getProcName(5), ENT_NAME("four"));
        requireEqual(manager.getProcName(6), ENT_NAME("four"));
    }

    SECTION("Retrieval of cfg (edge case in middle)") {
        requireEqualRef(manager.getCFG(1), test_one);
        requireEqualRef(manager.getCFG(2), test_one);
        requireEqualRef(manager.getCFG(3), test_two);
        requireEqualRef(manager.getCFG(4), test_three);
        requireEqualRef(manager.getCFG(5), test_four);
        requireEqualRef(manager.getCFG(6), test_four);
    }
}

TEST_CASE("5. Test storage and invalid retrieval in CFGManager") {
    CFG::CFGManager manager;
    CFG::CFGraph def;
    CFG::CFGraph one(def, 1, 2, "one");
    CFG::CFGraph two(def, 3, 3, "two");
    CFG::CFGraph three(def, 4, 4, "three");
    CFG::CFGraph four(def, 5, 6, "four");
    CFG::CFGraph test_one(def, 1, 2, "one");
    CFG::CFGraph test_two(def, 3, 3, "two");
    CFG::CFGraph test_three(def, 4, 4, "three");
    CFG::CFGraph test_four(def, 5, 6, "four");
    std::vector<CFG::CFGraph> test;
    test.emplace_back(std::move(one));
    test.emplace_back(std::move(two));
    test.emplace_back(std::move(three));
    test.emplace_back(std::move(four));
    manager.setGraphs(std::move(test));
    SECTION("Retrieval of procedure names with invalid index") {
        requireFalse(manager.isValidStmtNum(0));
        requireFalse(manager.isValidStmtNum(7));
    }
}
