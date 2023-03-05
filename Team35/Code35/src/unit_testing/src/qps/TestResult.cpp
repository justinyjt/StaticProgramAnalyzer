#include "catch.hpp"
#include "../TestHelper.h"
#include "qps/result/TableResult.h"
#include "qps/result/BoolResult.h"

TEST_CASE("Test join: 1 common column") {
    std::list<std::string> header1 = {"s", "v"};
    std::vector<std::list<std::string>> rows1;
    rows1.push_back({"1", "x"});
    rows1.push_back({"1", "y"});
    std::unique_ptr<TableResult> tableResult1 = std::make_unique<TableResult>(header1, rows1);

    std::list<std::string> header2 = {"a", "v"};
    std::vector<std::list<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"2", "x"});
    rows2.push_back({"1", "z"});
    std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

    std::list<std::string> resultHeader = {"v", "s", "a"};
    std::vector<std::list<std::string>> resultRows;
    resultRows.push_back({"x", "1", "1"});
    resultRows.push_back({"x", "1", "2"});
    std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>(resultHeader, resultRows);

    requireTrue(*expectedResult == *(Result::join(*tableResult1, *tableResult2)));
}

TEST_CASE("Test join: true and true") {
    std::unique_ptr<BoolResult> boolResult1 = std::make_unique<BoolResult>(true);

    std::unique_ptr<BoolResult> boolResult2 = std::make_unique<BoolResult>(true);

    std::unique_ptr<BoolResult> expectedResult = std::make_unique<BoolResult>(true);

    requireTrue(*expectedResult == *(Result::join(*boolResult1, *boolResult2)));
}

TEST_CASE("Test join: true and false") {
    std::unique_ptr<BoolResult> boolResult1 = std::make_unique<BoolResult>(true);

    std::unique_ptr<BoolResult> boolResult2 = std::make_unique<BoolResult>(false);

    std::unique_ptr<BoolResult> expectedResult = std::make_unique<BoolResult>(false);

    requireTrue(*expectedResult == *(Result::join(*boolResult1, *boolResult2)));
}

TEST_CASE("Test join: false and true") {
    std::unique_ptr<BoolResult> boolResult1 = std::make_unique<BoolResult>(false);

    std::unique_ptr<BoolResult> boolResult2 = std::make_unique<BoolResult>(true);

    std::unique_ptr<BoolResult> expectedResult = std::make_unique<BoolResult>(false);

    requireTrue(*expectedResult == *(Result::join(*boolResult1, *boolResult2)));
}

TEST_CASE("Test join: false and false") {
    std::unique_ptr<BoolResult> boolResult1 = std::make_unique<BoolResult>(true);

    std::unique_ptr<BoolResult> boolResult2 = std::make_unique<BoolResult>(true);

    std::unique_ptr<BoolResult> expectedResult = std::make_unique<BoolResult>(true);

    requireTrue(*expectedResult == *(Result::join(*boolResult1, *boolResult2)));
}

TEST_CASE("Test join: true and table") {
    std::unique_ptr<BoolResult> boolResult = std::make_unique<BoolResult>(true);

    std::list<std::string> header2 = {"a", "v"};
    std::vector<std::list<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"1", "z"});
    std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

    std::list<std::string> resultHeader = {"a", "v"};
    std::vector<std::list<std::string>> resultRows;
    resultRows.push_back({"1", "x"});
    resultRows.push_back({"1", "z"});
    std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>(resultHeader, resultRows);

    requireTrue(*expectedResult == *(Result::join(*boolResult, *tableResult2)));
}


TEST_CASE("Test join: false and table") {
    std::unique_ptr<BoolResult> boolResult = std::make_unique<BoolResult>(false);

    std::list<std::string> header2 = {"a", "v"};
    std::vector<std::list<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"1", "z"});
    std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

    std::unique_ptr<BoolResult> expectedResult = std::make_unique<BoolResult>(false);

    requireTrue(*expectedResult == *(Result::join(*boolResult, *tableResult2)));
}

TEST_CASE("Test join: 2 common columns") {
    std::list<std::string> header1 = {"s", "v", "c"};
    std::vector<std::list<std::string>> rows1;
    rows1.push_back({"1", "x", "1"});
    rows1.push_back({"1", "y", "5"});
    std::unique_ptr<TableResult> tableResult1 = std::make_unique<TableResult>(header1, rows1);

    std::list<std::string> header2 = {"a", "v", "c"};
    std::vector<std::list<std::string>> rows2;
    rows2.push_back({"1", "x", "2"});
    rows2.push_back({"2", "x", "2"});
    rows2.push_back({"1", "z", "3"});
    std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

    std::list<std::string> expectedHeader = {"v", "c", "s", "a"};
    std::vector<std::list<std::string>> expectedRows;
    std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>(expectedHeader, expectedRows);

    requireTrue(*expectedResult == *(Result::join(*tableResult1, *tableResult2)));
}


TEST_CASE("Test join: no common columns, 2x2") {
    std::list<std::string> header1 = {"s", "v"};
    std::vector<std::list<std::string>> rows1;
    rows1.push_back({"1", "x"});
    rows1.push_back({"1", "y"});
    std::unique_ptr<TableResult> tableResult1 = std::make_unique<TableResult>(header1, rows1);

    std::list<std::string> header2 = {"a", "b"};
    std::vector<std::list<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"2", "x"});
    std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

    std::list<std::string> resultHeader = {"s", "v", "a", "b"};
    std::vector<std::list<std::string>> resultRows;
    resultRows.push_back({"1", "x", "1", "x"});
    resultRows.push_back({"1", "x", "2", "x"});
    resultRows.push_back({"1", "y", "1", "x"});
    resultRows.push_back({"1", "y", "2", "x"});
    std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>(resultHeader, resultRows);

    requireTrue(*expectedResult == *(Result::join(*tableResult1, *tableResult2)));
}

TEST_CASE("Test join: no common columns, 1x2") {
    std::list<std::string> header1 = {"s"};
    std::vector<std::list<std::string>> rows1;
    rows1.push_back({"1"});
    rows1.push_back({"2"});
    std::unique_ptr<TableResult> tableResult1 = std::make_unique<TableResult>(header1, rows1);

    std::list<std::string> header2 = {"a", "b"};
    std::vector<std::list<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"2", "x"});
    std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

    std::list<std::string> resultHeader = {"s", "a", "b"};
    std::vector<std::list<std::string>> resultRows;
    resultRows.push_back({"1", "1", "x"});
    resultRows.push_back({"1", "2", "x"});
    resultRows.push_back({"2", "1", "x"});
    resultRows.push_back({"2", "2", "x"});
    std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>(resultHeader, resultRows);

    requireTrue(*expectedResult == *(Result::join(*tableResult1, *tableResult2)));
}

TEST_CASE("Test join: no common columns, 3x2") {
    std::list<std::string> header1 = {"s", "a", "y"};
    std::vector<std::list<std::string>> rows1;
    rows1.push_back({"1", "1", "4"});
    rows1.push_back({"2", "1", "4"});
    std::unique_ptr<TableResult> tableResult1 = std::make_unique<TableResult>(header1, rows1);

    std::list<std::string> header2 = {"a", "b"};
    std::vector<std::list<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"2", "x"});
    std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

    std::list<std::string> resultHeader = {"a", "s", "y", "b"};
    std::vector<std::list<std::string>> resultRows;
    resultRows.push_back({"1", "1", "4", "x"});
    resultRows.push_back({"1", "2", "4", "x"});
    std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>(resultHeader, resultRows);

    requireTrue(*expectedResult == *(Result::join(*tableResult1, *tableResult2)));
}

TEST_CASE("Test output") {
    std::list<std::string> result;
    std::list<std::string> header1 = {"a", "s", "y"};
    std::vector<std::list<std::string>> rows1;
    rows1.push_back({"1", "1", "4"});
    rows1.push_back({"2", "2", "4"});
    std::unique_ptr<TableResult> tableResult1 = std::make_unique<TableResult>(header1, rows1);

    std::list<std::string> header2 = {"a", "y"};
    std::vector<std::list<std::string>> rows2;
    rows2.push_back({"1", "4"});
    rows2.push_back({"1", "2"});
    std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

    std::unique_ptr<Result> resultTable = Result::join(*tableResult1, *tableResult2);

    std::list<std::string> outputHeaders = {"a", "y", "s"};
    std::vector<std::list<std::string>> outputRows;
    std::list<std::string> row;
    row.push_back("1");
    row.push_back("4");
    row.push_back("1");
    outputRows.push_back(row);
    std::unique_ptr<TableResult> outputTable = std::make_unique<TableResult>(outputHeaders, outputRows);
    requireTrue(*outputTable==*resultTable);
}

TEST_CASE("Test output selected does not match") {
    std::list<std::string> result;
    std::unique_ptr<BoolResult> boolResult = std::make_unique<BoolResult>(false);

    std::list<std::string> header2 = {"a", "b"};
    std::vector<std::list<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"2", "x"});
    std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

    std::unique_ptr<Result> resultTable = Result::join(*boolResult, *tableResult2);
    std::unique_ptr<BoolResult> outputTable = std::make_unique<BoolResult>(false);
    std::string col = "c";
    resultTable->output(result);

    requireTrue(*outputTable==*(Result::join(*boolResult, *tableResult2)));
}

TEST_CASE("Test output empty") {
    std::list<std::string> result;
    std::list<std::string> header2 = {"a"};
    std::vector<std::list<std::string>> rows2;
    std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

    std::string col = "a";
    tableResult2->output(result);

    requireTrue(result.size() == 0);
}
