#include "catch.hpp"
#include "../TestHelper.h"
#include "qps/result/TableResult.h"
#include "qps/result/SelectResult.h"
#include "qps/result/BoolResult.h"

TEST_CASE("TableResult join: 1 common column") {
    std::vector<std::string> header1 = {"s", "v"};
    std::vector<std::vector<std::string>> rows1;
    rows1.push_back({"1", "x"});
    rows1.push_back({"1", "y"});
    std::unique_ptr<TableResult> tableResult1 = std::make_unique<TableResult>(header1, rows1);

    std::vector<std::string> header2 = {"a", "v"};
    std::vector<std::vector<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"2", "x"});
    rows2.push_back({"1", "z"});
    std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

    std::vector<std::string> resultHeader = {"s", "v", "a"};
    std::vector<std::vector<std::string>> resultRows;
    resultRows.push_back({"1", "x", "1"});
    resultRows.push_back({"1", "x", "2"});
    std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>(resultHeader, resultRows);

    requireTrue(*expectedResult == *tableResult1->join(*tableResult2));
}

TEST_CASE("TableResult join: 2 common columns") {
    std::vector<std::string> header1 = {"s", "v", "c"};
    std::vector<std::vector<std::string>> rows1;
    rows1.push_back({"1", "x", "1"});
    rows1.push_back({"1", "y", "5"});
    std::unique_ptr<TableResult> tableResult1 = std::make_unique<TableResult>(header1, rows1);

    std::vector<std::string> header2 = {"a", "v", "c"};
    std::vector<std::vector<std::string>> rows2;
    rows2.push_back({"1", "x", "2"});
    rows2.push_back({"2", "x", "2"});
    rows2.push_back({"1", "z", "3"});
    std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

    std::vector<std::string> expectedHeader = {"s", "v", "c", "a"};
    std::vector<std::vector<std::string>> expectedRows;
    std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>(expectedHeader, expectedRows);

    requireTrue(*expectedResult == *tableResult1->join(*tableResult2));
}

TEST_CASE("TableResult join: no common columns, 2x2") {
    std::vector<std::string> header1 = {"s", "v"};
    std::vector<std::vector<std::string>> rows1;
    rows1.push_back({"1", "x"});
    rows1.push_back({"1", "y"});
    std::unique_ptr<TableResult> tableResult1 = std::make_unique<TableResult>(header1, rows1);

    std::vector<std::string> header2 = {"a", "b"};
    std::vector<std::vector<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"2", "x"});
    std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

    std::vector<std::string> resultHeader = {"s", "v", "a", "b"};
    std::vector<std::vector<std::string>> resultRows;
    resultRows.push_back({"1", "x", "1", "x"});
    resultRows.push_back({"1", "x", "2", "x"});
    resultRows.push_back({"1", "y", "1", "x"});
    resultRows.push_back({"1", "y", "2", "x"});
    std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>(resultHeader, resultRows);

    requireTrue(*expectedResult == *tableResult1->join(*tableResult2));
}

TEST_CASE("TableResult join: no common columns, 1x2") {
    std::vector<std::string> header1 = {"s"};
    std::vector<std::vector<std::string>> rows1;
    rows1.push_back({"1"});
    rows1.push_back({"2"});
    std::unique_ptr<TableResult> tableResult1 = std::make_unique<TableResult>(header1, rows1);

    std::vector<std::string> header2 = {"a", "b"};
    std::vector<std::vector<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"2", "x"});
    std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

    std::vector<std::string> resultHeader = {"s", "a", "b"};
    std::vector<std::vector<std::string>> resultRows;
    resultRows.push_back({"1", "1", "x"});
    resultRows.push_back({"1", "2", "x"});
    resultRows.push_back({"2", "1", "x"});
    resultRows.push_back({"2", "2", "x"});
    std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>(resultHeader, resultRows);

    requireTrue(*expectedResult == *tableResult1->join(*tableResult2));
}

TEST_CASE("TableResult join: no common columns, 3x2") {
    std::vector<std::string> header1 = {"s", "a", "y"};
    std::vector<std::vector<std::string>> rows1;
    rows1.push_back({"1", "1", "4"});
    rows1.push_back({"2", "1", "4"});
    std::unique_ptr<TableResult> tableResult1 = std::make_unique<TableResult>(header1, rows1);

    std::vector<std::string> header2 = {"a", "b"};
    std::vector<std::vector<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"2", "x"});
    std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

    std::vector<std::string> resultHeader = {"s", "a", "y", "b"};
    std::vector<std::vector<std::string>> resultRows;
    resultRows.push_back({"1", "1", "4", "x"});
    resultRows.push_back({"2", "1", "4", "x"});
    std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>(resultHeader, resultRows);

    std::unique_ptr<Result> res = tableResult1->join(*tableResult2);

    requireTrue(*expectedResult == *tableResult1->join(*tableResult2));
}

TEST_CASE("Test output selected does not match") {
    std::list<std::string> result;
    std::unique_ptr<BoolResult> boolResult = std::make_unique<BoolResult>(false);

    std::vector<std::string> header2 = {"a", "b"};
    std::vector<std::vector<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"2", "x"});
    std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

    std::unique_ptr<Result> resultTable = boolResult->join(*tableResult2);
    std::unique_ptr<BoolResult> outputTable = std::make_unique<BoolResult>(false);
    std::string col = "c";
    resultTable->output(result);

    requireTrue(*outputTable == *boolResult->join(*tableResult2));
}

TEST_CASE("Test output empty") {
    std::list<std::string> result;
    std::vector<std::string> header2 = {"a"};
    std::vector<std::vector<std::string>> rows2;
    std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

    std::string col = "a";
    tableResult2->output(result);

    requireTrue(result.size() == 0);
}

TEST_CASE("Test Boolean join") {
    SECTION("Test join: true and true") {
        std::unique_ptr<BoolResult> boolResult1 = std::make_unique<BoolResult>(true);

        std::unique_ptr<BoolResult> boolResult2 = std::make_unique<BoolResult>(true);

        std::unique_ptr<BoolResult> expectedResult = std::make_unique<BoolResult>(true);

        requireTrue(*expectedResult == *boolResult1->join(*boolResult2));
    }

    SECTION("Test join: true and false") {
        std::unique_ptr<BoolResult> boolResult1 = std::make_unique<BoolResult>(true);

        std::unique_ptr<BoolResult> boolResult2 = std::make_unique<BoolResult>(false);

        std::unique_ptr<BoolResult> expectedResult = std::make_unique<BoolResult>(false);

        requireTrue(*expectedResult == *boolResult1->join(*boolResult2));
    }

    SECTION("Test join: false and true") {
        std::unique_ptr<BoolResult> boolResult1 = std::make_unique<BoolResult>(false);

        std::unique_ptr<BoolResult> boolResult2 = std::make_unique<BoolResult>(true);

        std::unique_ptr<BoolResult> expectedResult = std::make_unique<BoolResult>(false);

        requireTrue(*expectedResult == *boolResult1->join(*boolResult2));
    }

    SECTION("Test join: false and false") {
        std::unique_ptr<BoolResult> boolResult1 = std::make_unique<BoolResult>(true);

        std::unique_ptr<BoolResult> boolResult2 = std::make_unique<BoolResult>(true);

        std::unique_ptr<BoolResult> expectedResult = std::make_unique<BoolResult>(true);

        requireTrue(*expectedResult == *boolResult1->join(*boolResult2));
    }

    SECTION("Test join: true and table") {
        std::unique_ptr<BoolResult> boolResult = std::make_unique<BoolResult>(true);

        std::vector<std::string> header2 = {"a", "v"};
        std::vector<std::vector<std::string>> rows2;
        rows2.push_back({"1", "x"});
        rows2.push_back({"1", "z"});
        std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

        std::vector<std::string> resultHeader = {"a", "v"};
        std::vector<std::vector<std::string>> resultRows;
        resultRows.push_back({"1", "x"});
        resultRows.push_back({"1", "z"});
        std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>(resultHeader, resultRows);

        requireTrue(*expectedResult == *boolResult->join(*tableResult2));
    }

    SECTION("Test join: false and table") {
        std::unique_ptr<BoolResult> boolResult = std::make_unique<BoolResult>(false);

        std::vector<std::string> header2 = {"a", "v"};
        std::vector<std::vector<std::string>> rows2;
        rows2.push_back({"1", "x"});
        rows2.push_back({"1", "z"});
        std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

        std::unique_ptr<BoolResult> expectedResult = std::make_unique<BoolResult>(false);

        requireTrue(*expectedResult == *boolResult->join(*tableResult2));
    }

    SECTION("Test join: table and true") {
        std::unique_ptr<BoolResult> boolResult = std::make_unique<BoolResult>(true);

        std::vector<std::string> header2 = {"a", "v"};
        std::vector<std::vector<std::string>> rows2;
        rows2.push_back({"1", "x"});
        rows2.push_back({"1", "z"});
        std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

        std::vector<std::string> resultHeader = {"a", "v"};
        std::vector<std::vector<std::string>> resultRows;
        resultRows.push_back({"1", "x"});
        resultRows.push_back({"1", "z"});
        std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>(resultHeader, resultRows);

        requireTrue(*expectedResult == *tableResult2->join(*boolResult));
    }

    SECTION("Test join: table and true") {
        std::unique_ptr<BoolResult> boolResult = std::make_unique<BoolResult>(false);

        std::vector<std::string> header2 = {"a", "v"};
        std::vector<std::vector<std::string>> rows2;
        rows2.push_back({"1", "x"});
        rows2.push_back({"1", "z"});
        std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

        std::unique_ptr<BoolResult> expectedResult = std::make_unique<BoolResult>(false);

        requireTrue(*expectedResult == *tableResult2->join(*boolResult));
    }

    SECTION("Test join: select boolean, BOOLEAN and non-empty table") {
        std::unique_ptr<BoolResult> boolResult = std::make_unique<BoolResult>(true, true);

        std::vector<std::string> header2 = {"a", "v"};
        std::vector<std::vector<std::string>> rows2;
        rows2.push_back({"1", "x"});
        rows2.push_back({"1", "z"});
        std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

        std::unique_ptr<BoolResult> expectedResult = std::make_unique<BoolResult>(true);

        requireTrue(*expectedResult == *boolResult->join(*tableResult2));
    }

    SECTION("Test join: select boolean, BOOLEAN and empty table") {
        std::unique_ptr<BoolResult> boolResult = std::make_unique<BoolResult>(false);

        std::vector<std::string> header2 = {"a", "v"};
        std::vector<std::vector<std::string>> rows2;
        std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

        std::unique_ptr<BoolResult> expectedResult = std::make_unique<BoolResult>(false);

        requireTrue(*expectedResult == *boolResult->join(*tableResult2));
    }
}

TEST_CASE("Test Select join") {

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
        std::unique_ptr<TableResult> intermediateResult = std::make_unique<TableResult>(intermediateTableHeader, intermediateTableRow);
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

    }

    SECTION("2 common headers in results, select result table larger") {

    }

    SECTION("1 common header in results, exact match") {

    }

    SECTION("2 common headers in results, exact match, same order") {

    }

    SECTION("2 common headers in results, exact match, different order") {

    }

    SECTION("2 common headers in results, intermediate result table larger, same order") {

    }

    SECTION("2 common headers in results, intermediate result table larger, different order") {

    }
}
