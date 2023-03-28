#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/result/TableResult.h"

TEST_CASE("Table join") {
    SECTION("1 common column") {
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

    SECTION("2 common columns") {
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

    SECTION("2 common columns, different order") {
        std::vector<std::string> header1 = {"s", "c", "v"};
        std::vector<std::vector<std::string>> rows1;
        rows1.push_back({"1", "2", "x"});
        rows1.push_back({"1", "5", "y"});
        std::unique_ptr<TableResult> tableResult1 = std::make_unique<TableResult>(header1, rows1);

        std::vector<std::string> header2 = {"a", "v", "c"};
        std::vector<std::vector<std::string>> rows2;
        rows2.push_back({"1", "x", "2"});
        rows2.push_back({"2", "x", "2"});
        rows2.push_back({"1", "z", "3"});
        std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

        std::vector<std::string> expectedHeader = {"s", "c", "v", "a"};
        std::vector<std::vector<std::string>> expectedRows;
        expectedRows.push_back({"1", "2", "x", "1"});
        expectedRows.push_back({"1", "2", "x", "2"});
        std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>(expectedHeader, expectedRows);
        std::unique_ptr<Result> res = tableResult1->join(*tableResult2);

        requireTrue(*expectedResult == *tableResult1->join(*tableResult2));
    }

    SECTION("no common columns, 2x2") {
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

    SECTION("no common columns, 1x2") {
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
}
