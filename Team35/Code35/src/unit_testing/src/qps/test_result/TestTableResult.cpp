#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/result/TableResult.h"
#include "qps/result/BoolResult.h"

TEST_CASE("Table join") {
    SECTION("1 common column") {
        TableHeaders header1 = {"s", "v"};
        TableRows rows1;
        rows1.push_back({"1", "x"});
        rows1.push_back({"1", "y"});
        std::unique_ptr<TableResult> tableResult1 = std::make_unique<TableResult>(header1, rows1);

        TableHeaders header2 = {"a", "v"};
        TableRows rows2;
        rows2.push_back({"1", "x"});
        rows2.push_back({"2", "x"});
        rows2.push_back({"1", "z"});
        std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

        TableHeaders resultHeader = {"s", "v", "a"};
        TableRows resultRows;
        resultRows.push_back({"1", "x", "1"});
        resultRows.push_back({"1", "x", "2"});
        std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>(resultHeader, resultRows);

        requireTrue(*expectedResult == *tableResult1->join(*tableResult2));
    }

    SECTION("2 common columns") {
        TableHeaders header1 = {"s", "v", "c"};
        TableRows rows1;
        rows1.push_back({"1", "x", "1"});
        rows1.push_back({"1", "y", "5"});
        std::unique_ptr<TableResult> tableResult1 = std::make_unique<TableResult>(header1, rows1);

        TableHeaders header2 = {"a", "v", "c"};
        TableRows rows2;
        rows2.push_back({"1", "x", "2"});
        rows2.push_back({"2", "x", "2"});
        rows2.push_back({"1", "z", "3"});
        std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

        TableHeaders expectedHeader = {"s", "v", "c", "a"};
        TableRows expectedRows;
        std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>(expectedHeader, expectedRows);

        requireTrue(*expectedResult == *tableResult1->join(*tableResult2));
    }

    SECTION("2 common columns, different order") {
        TableHeaders header1 = {"s", "c", "v"};
        TableRows rows1;
        rows1.push_back({"1", "2", "x"});
        rows1.push_back({"1", "5", "y"});
        std::unique_ptr<TableResult> tableResult1 = std::make_unique<TableResult>(header1, rows1);

        TableHeaders header2 = {"a", "v", "c"};
        TableRows rows2;
        rows2.push_back({"1", "x", "2"});
        rows2.push_back({"2", "x", "2"});
        rows2.push_back({"1", "z", "3"});
        std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

        TableHeaders expectedHeader = {"s", "c", "v", "a"};
        TableRows expectedRows;
        expectedRows.push_back({"1", "2", "x", "1"});
        expectedRows.push_back({"1", "2", "x", "2"});
        std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>(expectedHeader, expectedRows);
        std::unique_ptr<Result> res = tableResult1->join(*tableResult2);

        requireTrue(*expectedResult == *tableResult1->join(*tableResult2));
    }

    SECTION("no common columns, 2x2") {
        TableHeaders header1 = {"s", "v"};
        TableRows rows1;
        rows1.push_back({"1", "x"});
        rows1.push_back({"1", "y"});
        std::unique_ptr<TableResult> tableResult1 = std::make_unique<TableResult>(header1, rows1);

        TableHeaders header2 = {"a", "b"};
        TableRows rows2;
        rows2.push_back({"1", "x"});
        rows2.push_back({"2", "x"});
        std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

        TableHeaders resultHeader = {"s", "v", "a", "b"};
        TableRows resultRows;
        resultRows.push_back({"1", "x", "1", "x"});
        resultRows.push_back({"1", "x", "2", "x"});
        resultRows.push_back({"1", "y", "1", "x"});
        resultRows.push_back({"1", "y", "2", "x"});
        std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>(resultHeader, resultRows);

        requireTrue(*expectedResult == *tableResult1->join(*tableResult2));
    }

    SECTION("no common columns, 1x2") {
        TableHeaders header1 = {"s"};
        TableRows rows1;
        rows1.push_back({"1"});
        rows1.push_back({"2"});
        std::unique_ptr<TableResult> tableResult1 = std::make_unique<TableResult>(header1, rows1);

        TableHeaders header2 = {"a", "b"};
        TableRows rows2;
        rows2.push_back({"1", "x"});
        rows2.push_back({"2", "x"});
        std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

        TableHeaders resultHeader = {"s", "a", "b"};
        TableRows resultRows;
        resultRows.push_back({"1", "1", "x"});
        resultRows.push_back({"1", "2", "x"});
        resultRows.push_back({"2", "1", "x"});
        resultRows.push_back({"2", "2", "x"});
        std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>(resultHeader, resultRows);

        requireTrue(*expectedResult == *tableResult1->join(*tableResult2));
    }

    SECTION("bool result true") {
        TableHeaders header1 = {"s"};
        TableRows rows1;
        rows1.push_back({"1"});
        rows1.push_back({"2"});
        std::unique_ptr<TableResult> tableResult1 = std::make_unique<TableResult>(header1, rows1);

        std::unique_ptr<BoolResult> boolResult = std::make_unique<BoolResult>(true);

        TableHeaders resultHeader = {"s"};
        TableRows resultRows;
        resultRows.push_back({"1"});
        resultRows.push_back({"2"});
        std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>(resultHeader, resultRows);

        requireTrue(*expectedResult == *tableResult1->join(*boolResult));
    }

    SECTION("bool result false") {
        TableHeaders header1 = {"s"};
        TableRows rows1;
        rows1.push_back({"1"});
        rows1.push_back({"2"});
        std::unique_ptr<TableResult> tableResult1 = std::make_unique<TableResult>(header1, rows1);

        std::unique_ptr<BoolResult> boolResult = std::make_unique<BoolResult>(false);

        std::unique_ptr<TableResult> expectedResult = std::make_unique<TableResult>();

        requireTrue(*expectedResult == *tableResult1->join(*boolResult));
    }
}
