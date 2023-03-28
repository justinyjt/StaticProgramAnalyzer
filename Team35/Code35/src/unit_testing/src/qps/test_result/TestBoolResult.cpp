#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/result/BoolResult.h"

TEST_CASE("Boolean join") {
    SECTION("true and true") {
        std::unique_ptr<BoolResult> boolResult1 = std::make_unique<BoolResult>(true);
        std::unique_ptr<BoolResult> boolResult2 = std::make_unique<BoolResult>(true);
        std::unique_ptr<BoolResult> expectedResult = std::make_unique<BoolResult>(true);

        requireTrue(*expectedResult == *boolResult1->join(*boolResult2));
    }

    SECTION("true and false") {
        std::unique_ptr<BoolResult> boolResult1 = std::make_unique<BoolResult>(true);
        std::unique_ptr<BoolResult> boolResult2 = std::make_unique<BoolResult>(false);
        std::unique_ptr<BoolResult> expectedResult = std::make_unique<BoolResult>(false);

        requireTrue(*expectedResult == *boolResult1->join(*boolResult2));
    }

    SECTION("false and true") {
        std::unique_ptr<BoolResult> boolResult1 = std::make_unique<BoolResult>(false);
        std::unique_ptr<BoolResult> boolResult2 = std::make_unique<BoolResult>(true);
        std::unique_ptr<BoolResult> expectedResult = std::make_unique<BoolResult>(false);

        requireTrue(*expectedResult == *boolResult1->join(*boolResult2));
    }

    SECTION("false and false") {
        std::unique_ptr<BoolResult> boolResult1 = std::make_unique<BoolResult>(true);
        std::unique_ptr<BoolResult> boolResult2 = std::make_unique<BoolResult>(true);
        std::unique_ptr<BoolResult> expectedResult = std::make_unique<BoolResult>(true);

        requireTrue(*expectedResult == *boolResult1->join(*boolResult2));
    }

    SECTION("true and table") {
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

    SECTION("false and table") {
        std::unique_ptr<BoolResult> boolResult = std::make_unique<BoolResult>(false);

        std::vector<std::string> header2 = {"a", "v"};
        std::vector<std::vector<std::string>> rows2;
        rows2.push_back({"1", "x"});
        rows2.push_back({"1", "z"});
        std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

        std::unique_ptr<BoolResult> expectedResult = std::make_unique<BoolResult>(false);

        requireTrue(*expectedResult == *boolResult->join(*tableResult2));
    }

    SECTION("table and true") {
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

    SECTION("Select boolean, non-empty intermediate") {
        std::unique_ptr<BoolResult> boolResult = std::make_unique<BoolResult>(true, true);

        std::vector<std::string> header2 = {"a", "v"};
        std::vector<std::vector<std::string>> rows2;
        rows2.push_back({"1", "x"});
        rows2.push_back({"1", "z"});
        std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

        std::unique_ptr<BoolResult> expectedResult = std::make_unique<BoolResult>(true);

        requireTrue(*expectedResult == *boolResult->join(*tableResult2));
    }

    SECTION("Select boolean, empty intermediate") {
        std::unique_ptr<BoolResult> boolResult = std::make_unique<BoolResult>(false, true);

        std::vector<std::string> header2 = {"a", "v"};
        std::vector<std::vector<std::string>> rows2;
        std::unique_ptr<TableResult> tableResult2 = std::make_unique<TableResult>(header2, rows2);

        std::unique_ptr<BoolResult> expectedResult = std::make_unique<BoolResult>(false);

        requireTrue(*expectedResult == *boolResult->join(*tableResult2));
    }
}
