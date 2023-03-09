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
    std::vector<CFG::CFGraph> test = {one, two, three, four};
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
        requireEqual(manager.getCFG(1), one);
        requireEqual(manager.getCFG(2), one);
        requireEqual(manager.getCFG(3), one);
        requireEqual(manager.getCFG(4), two);
        requireEqual(manager.getCFG(5), two);
        requireEqual(manager.getCFG(6), two);
        requireEqual(manager.getCFG(7), two);
        requireEqual(manager.getCFG(8), three);
        requireEqual(manager.getCFG(9), three);
        requireEqual(manager.getCFG(10), three);
        requireEqual(manager.getCFG(11), three);
        requireEqual(manager.getCFG(12), three);
        requireEqual(manager.getCFG(13), three);
        requireEqual(manager.getCFG(14), three);
        requireEqual(manager.getCFG(15), three);
        requireEqual(manager.getCFG(16), four);

    }
}

TEST_CASE("2. Test storage and retrieval in CFGManager") {
    CFG::CFGManager manager;
    CFG::CFGraph def;
    CFG::CFGraph one(def, 1, 3, "one");
    CFG::CFGraph two(def, 4, 4, "two");
    CFG::CFGraph three(def, 5, 10, "three");
    std::vector<CFG::CFGraph> test = {one, two, three};
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
        requireEqual(manager.getCFG(1), one);
        requireEqual(manager.getCFG(2), one);
        requireEqual(manager.getCFG(3), one);
        requireEqual(manager.getCFG(4), two);
        requireEqual(manager.getCFG(5), three);
        requireEqual(manager.getCFG(6), three);
        requireEqual(manager.getCFG(7), three);
        requireEqual(manager.getCFG(8), three);
        requireEqual(manager.getCFG(9), three);
        requireEqual(manager.getCFG(10), three);

    }
}

TEST_CASE("3. Test storage and retrieval in CFGManager") {
    CFG::CFGManager manager;
    CFG::CFGraph def;
    CFG::CFGraph one(def, 1, 4, "one");
    CFG::CFGraph two(def, 5, 5, "two");
    CFG::CFGraph three(def, 5, 10, "three");
    std::vector<CFG::CFGraph> test = {one, two, three};
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
        requireEqual(manager.getCFG(1), one);
        requireEqual(manager.getCFG(2), one);
        requireEqual(manager.getCFG(3), one);
        requireEqual(manager.getCFG(4), one);
        requireEqual(manager.getCFG(5), two);
        requireEqual(manager.getCFG(6), three);
        requireEqual(manager.getCFG(7), three);
        requireEqual(manager.getCFG(8), three);
        requireEqual(manager.getCFG(9), three);
        requireEqual(manager.getCFG(10), three);

    }
}

TEST_CASE("4. Test storage and retrieval in CFGManager") {
    CFG::CFGManager manager;
    CFG::CFGraph def;
    CFG::CFGraph one(def, 1, 2, "one");
    CFG::CFGraph two(def, 3, 3, "two");
    CFG::CFGraph three(def, 4, 4, "three");
    CFG::CFGraph four(def, 5, 6, "four");
    std::vector<CFG::CFGraph> test = {one, two, three, four};
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
        requireEqual(manager.getCFG(1), one);
        requireEqual(manager.getCFG(2), one);
        requireEqual(manager.getCFG(3), two);
        requireEqual(manager.getCFG(4), three);
        requireEqual(manager.getCFG(5), four);
        requireEqual(manager.getCFG(6), four);
    }
}