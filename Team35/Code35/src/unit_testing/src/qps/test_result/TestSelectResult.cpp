#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/result/SelectResult.h"
#include "qps/result/IdentityResult.h"
#include "qps/result/BoolResult.h"

TEST_CASE("Select join") {
    std::vector<std::string> selectHeaders1 = {"x"};
    std::vector<std::string> selectHeaders2 = {"x", "y"};
    std::vector<std::string> selectHeaders3 = {"x", "y", "z"};

    std::vector<std::string> selectTableHeader1 = {"x"};
    std::vector<std::vector<std::string>> selectTableRows1;
    selectTableRows1.push_back({"1"});
    selectTableRows1.push_back({"2"});
    selectTableRows1.push_back({"3"});
    TableResult selectTable1(selectTableHeader1, selectTableRows1);

    std::vector<std::string> selectTableHeader2 = {"y"};
    std::vector<std::vector<std::string>> selectTableRows2;
    selectTableRows2.push_back({"4"});
    selectTableRows2.push_back({"3"});
    selectTableRows2.push_back({"2"});
    selectTableRows2.push_back({"1"});
    TableResult selectTable2(selectTableHeader2, selectTableRows2);

    std::vector<std::string> selectTableHeader3 = {"z"};
    std::vector<std::vector<std::string>> selectTableRows3;
    selectTableRows3.push_back({"a"});
    selectTableRows3.push_back({"b"});
    selectTableRows3.push_back({"c"});
    selectTableRows3.push_back({"d"});
    TableResult selectTable3(selectTableHeader3, selectTableRows3);

    std::vector<TableResult> selectTables1;
    selectTables1.push_back(selectTable1);
    std::unique_ptr<SelectResult> selectResult1 = std::make_unique<SelectResult>(selectHeaders1, selectTables1);
    std::vector<TableResult> selectTables2;
    selectTables2.push_back(selectTable1);
    selectTables2.push_back(selectTable2);
    std::unique_ptr<SelectResult> selectResult2 = std::make_unique<SelectResult>(selectHeaders2, selectTables2);
    std::vector<TableResult> selectTables3;
    selectTables3.push_back(selectTable1);
    selectTables3.push_back(selectTable2);
    selectTables3.push_back(selectTable3);
    std::unique_ptr<SelectResult> selectResult3 = std::make_unique<SelectResult>(selectHeaders3, selectTables3);


    SECTION("No common headers in results") {
        std::vector<std::string> intermediateTableHeader = {"a"};
        std::vector<std::vector<std::string>> intermediateTableRow;
        intermediateTableRow.push_back({"1"});
        intermediateTableRow.push_back({"2"});
        intermediateTableRow.push_back({"3"});
        intermediateTableRow.push_back({"4"});
        std::unique_ptr<TableResult> intermediateResult =
                std::make_unique<TableResult>(intermediateTableHeader, intermediateTableRow);
        std::unique_ptr<Result> res = selectResult1->join(*intermediateResult);
        std::list<std::string> result;
        res->output(result);
        std::list<std::string> expected;
        expected.push_back("1");
        expected.push_back("2");
        expected.push_back("3");

        requireTrue(expected == result);
    }

    SECTION("1 common header in results, select result table larger") {
        std::vector<std::string> intermediateTableHeader = {"x"};
        std::vector<std::vector<std::string>> intermediateTableRow;
        intermediateTableRow.push_back({"1"});
        intermediateTableRow.push_back({"2"});
        std::unique_ptr<TableResult> intermediateResult =
                std::make_unique<TableResult>(intermediateTableHeader, intermediateTableRow);
        std::unique_ptr<Result> res = selectResult2->join(*intermediateResult);
        std::list<std::string> result;
        res->output(result);
        std::list<std::string> expected;
        expected.push_back("1 1");
        expected.push_back("1 2");
        expected.push_back("1 3");
        expected.push_back("1 4");
        expected.push_back("2 1");
        expected.push_back("2 2");
        expected.push_back("2 3");
        expected.push_back("2 4");

        requireTrue(expected == result);
    }

    SECTION("2 common headers in results, select result table larger") {
        std::vector<std::string> intermediateTableHeader = {"x", "y"};
        std::vector<std::vector<std::string>> intermediateTableRow;
        intermediateTableRow.push_back({"1", "1"});
        intermediateTableRow.push_back({"2", "2"});
        std::unique_ptr<TableResult> intermediateResult =
                std::make_unique<TableResult>(intermediateTableHeader, intermediateTableRow);
        std::unique_ptr<Result> res = selectResult3->join(*intermediateResult);
        std::list<std::string> result;
        res->output(result);
        std::list<std::string> expected;
        expected.push_back("1 1 a");
        expected.push_back("1 1 b");
        expected.push_back("1 1 c");
        expected.push_back("1 1 d");
        expected.push_back("2 2 a");
        expected.push_back("2 2 b");
        expected.push_back("2 2 c");
        expected.push_back("2 2 d");

        requireTrue(expected == result);
    }

    SECTION("1 common header in results, exact match") {
        std::vector<std::string> intermediateTableHeader = {"x"};
        std::vector<std::vector<std::string>> intermediateTableRow;
        intermediateTableRow.push_back({"1"});
        intermediateTableRow.push_back({"2"});
        std::unique_ptr<TableResult> intermediateResult =
                std::make_unique<TableResult>(intermediateTableHeader, intermediateTableRow);
        std::unique_ptr<Result> res = selectResult1->join(*intermediateResult);
        std::list<std::string> result;
        res->output(result);
        std::list<std::string> expected;
        expected.push_back("1");
        expected.push_back("2");

        requireTrue(expected == result);
    }

    SECTION("2 common headers in results, exact match, same order") {
        std::vector<std::string> intermediateTableHeader = {"x", "y"};
        std::vector<std::vector<std::string>> intermediateTableRow;
        intermediateTableRow.push_back({"1", "4"});
        intermediateTableRow.push_back({"2", "3"});
        std::unique_ptr<TableResult> intermediateResult =
                std::make_unique<TableResult>(intermediateTableHeader, intermediateTableRow);
        std::unique_ptr<Result> res = selectResult2->join(*intermediateResult);
        std::list<std::string> result;
        res->output(result);
        std::list<std::string> expected;
        expected.push_back("1 4");
        expected.push_back("2 3");

        requireTrue(expected == result);
    }

    SECTION("2 common headers in results, exact match, different order") {
        std::vector<std::string> intermediateTableHeader = {"y", "x"};
        std::vector<std::vector<std::string>> intermediateTableRow;
        intermediateTableRow.push_back({"4", "1"});
        intermediateTableRow.push_back({"3", "2"});
        std::unique_ptr<TableResult> intermediateResult =
                std::make_unique<TableResult>(intermediateTableHeader, intermediateTableRow);
        std::unique_ptr<Result> res = selectResult2->join(*intermediateResult);
        std::list<std::string> result;
        res->output(result);
        std::list<std::string> expected;
        expected.push_back("2 3");
        expected.push_back("1 4");

        requireTrue(expected == result);
    }

    SECTION("2 common headers in results, intermediate result table larger, same order") {
        std::vector<std::string> intermediateTableHeader = {"x", "y", "z"};
        std::vector<std::vector<std::string>> intermediateTableRow;
        intermediateTableRow.push_back({"1", "4", "a"});
        intermediateTableRow.push_back({"2", "3", "b"});
        std::unique_ptr<TableResult> intermediateResult =
                std::make_unique<TableResult>(intermediateTableHeader, intermediateTableRow);
        std::unique_ptr<Result> res = selectResult2->join(*intermediateResult);
        std::list<std::string> result;
        res->output(result);
        std::list<std::string> expected;
        expected.push_back("1 4");
        expected.push_back("2 3");

        requireTrue(expected == result);
    }

    SECTION("2 common headers in results, intermediate result table larger, different order") {
        std::vector<std::string> intermediateTableHeader = {"z", "y", "x"};
        std::vector<std::vector<std::string>> intermediateTableRow;
        intermediateTableRow.push_back({"a", "4", "1"});
        intermediateTableRow.push_back({"b", "3", "2"});
        std::unique_ptr<TableResult> intermediateResult =
                std::make_unique<TableResult>(intermediateTableHeader, intermediateTableRow);
        std::unique_ptr<Result> res = selectResult2->join(*intermediateResult);
        std::list<std::string> result;
        res->output(result);
        std::list<std::string> expected;
        expected.push_back("2 3");
        expected.push_back("1 4");

        requireTrue(expected == result);
    }

    SECTION("identity result") {
        std::unique_ptr<IdentityResult> intermediateResult = std::make_unique<IdentityResult>();
        std::unique_ptr<Result> res = selectResult2->join(*intermediateResult);
        std::list<std::string> result;
        res->output(result);
        std::list<std::string> expected;
        expected.push_back("1 4");
        expected.push_back("1 3");
        expected.push_back("1 2");
        expected.push_back("1 1");
        expected.push_back("2 4");
        expected.push_back("2 3");
        expected.push_back("2 2");
        expected.push_back("2 1");
        expected.push_back("3 4");
        expected.push_back("3 3");
        expected.push_back("3 2");
        expected.push_back("3 1");

        requireTrue(expected == result);
    }

    SECTION("bool result true") {
        std::unique_ptr<BoolResult> intermediateResult = std::make_unique<BoolResult>(true);
        std::unique_ptr<Result> res = selectResult2->join(*intermediateResult);
        std::list<std::string> result;
        res->output(result);
        std::list<std::string> expected;
        expected.push_back("1 4");
        expected.push_back("1 3");
        expected.push_back("1 2");
        expected.push_back("1 1");
        expected.push_back("2 4");
        expected.push_back("2 3");
        expected.push_back("2 2");
        expected.push_back("2 1");
        expected.push_back("3 4");
        expected.push_back("3 3");
        expected.push_back("3 2");
        expected.push_back("3 1");

        requireTrue(expected == result);
    }

    SECTION("bool result false") {
        std::unique_ptr<BoolResult> intermediateResult = std::make_unique<BoolResult>(false);
        std::unique_ptr<Result> res = selectResult2->join(*intermediateResult);
        std::list<std::string> result;
        res->output(result);
        std::list<std::string> expected;

        requireTrue(expected == result);
    }
}
