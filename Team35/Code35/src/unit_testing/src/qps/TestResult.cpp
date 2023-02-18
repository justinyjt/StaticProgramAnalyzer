#include "catch.hpp"
#include "../TestHelper.h"
#include "qps/result/TableResult.h"
#include "qps/result/BoolResult.h"
#include "qps/query_parser/QueryParser.h"
#include "commons/lexer/ILexer.h"
#include "commons/lexer/LexerFactory.h"

TEST_CASE("Test join: 1 common column") {
    std::list<std::string> header1 = {"s", "v"};
    std::vector<std::list<std::string>> rows1;
    rows1.push_back({"1", "x"});
    rows1.push_back({"1", "y"});
    TableResult* tableResult1 = new TableResult(header1, rows1);

    std::list<std::string> header2 = {"a", "v"};
    std::vector<std::list<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"2", "x"});
    rows2.push_back({"1", "z"});
    TableResult* tableResult2 = new TableResult(header2, rows2);

    std::list<std::string> resultHeader = {"v", "s", "a"};
    std::vector<std::list<std::string>> resultRows;
    resultRows.push_back({"x", "1", "1"});
    resultRows.push_back({"x", "1", "2"});
    TableResult* expectedResult = new TableResult(resultHeader, resultRows);

    requireTrue(*expectedResult == *(Result::join(tableResult1, tableResult2)));
}

TEST_CASE("Test join: true and true") {
    BoolResult* boolResult1 = new BoolResult(true);

    BoolResult* boolResult2 = new BoolResult(true);

    BoolResult* expectedResult = new BoolResult(true);

    requireTrue(*expectedResult == *(Result::join(boolResult1, boolResult2)));
}

TEST_CASE("Test join: true and false") {
    BoolResult* boolResult1 = new BoolResult(true);

    BoolResult* boolResult2 = new BoolResult(false);

    BoolResult* expectedResult = new BoolResult(false);

    requireTrue(*expectedResult == *(Result::join(boolResult1, boolResult2)));
}

TEST_CASE("Test join: false and true") {
    BoolResult* boolResult1 = new BoolResult(false);

    BoolResult* boolResult2 = new BoolResult(true);

    BoolResult* expectedResult = new BoolResult(false);

    requireTrue(*expectedResult == *(Result::join(boolResult1, boolResult2)));
}

TEST_CASE("Test join: false and false") {
    BoolResult* boolResult1 = new BoolResult(false);

    BoolResult* boolResult2 = new BoolResult(false);

    BoolResult* expectedResult = new BoolResult(false);

    requireTrue(*expectedResult == *(Result::join(boolResult1, boolResult2)));
}

TEST_CASE("Test join: true and table") {
    BoolResult* boolResult = new BoolResult(true);

    std::list<std::string> header2 = {"a", "v"};
    std::vector<std::list<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"1", "z"});
    TableResult* tableResult2 = new TableResult(header2, rows2);

    std::list<std::string> resultHeader = {"a", "v"};
    std::vector<std::list<std::string>> resultRows;
    resultRows.push_back({"1", "x"});
    resultRows.push_back({"1", "z"});
    TableResult* expectedResult = new TableResult(resultHeader, resultRows);

    requireTrue(*expectedResult == *(Result::join(boolResult, tableResult2)));
}

TEST_CASE("Test join: false and table") {
    BoolResult* boolResult = new BoolResult(false);

    std::list<std::string> header2 = {"a", "v"};
    std::vector<std::list<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"1", "z"});
    TableResult* tableResult2 = new TableResult(header2, rows2);

    BoolResult* expectedResult = new BoolResult(false);

    requireTrue(*expectedResult == *(Result::join(boolResult, tableResult2)));
}

/*
TEST_CASE("Test join: 2 common columns") {
    std::list<std::string> header1 = {"s", "v", "c"};
    std::vector<std::list<std::string>> rows1;
    rows1.push_back({"1", "x", "1"});
    rows1.push_back({"1", "y", "5"});
    TableResult* tableResult1 = new TableResult(header1, rows1);

    std::list<std::string> header2 = {"a", "v", "c"};
    std::vector<std::list<std::string>> rows2;
    rows2.push_back({"1", "x", "2"});
    rows2.push_back({"2", "x", "2"});
    rows2.push_back({"1", "z", "3"});
    TableResult* tableResult2 = new TableResult(header2, rows2);

    BoolResult* expectedResult = new BoolResult(false);

    requireTrue(*expectedResult == *(Result::join(tableResult1, tableResult2)));
}
*/

TEST_CASE("Test join: no common columns, 2x2") {
    std::list<std::string> header1 = {"s", "v"};
    std::vector<std::list<std::string>> rows1;
    rows1.push_back({"1", "x"});
    rows1.push_back({"1", "y"});
    TableResult* tableResult1 = new TableResult(header1, rows1);

    std::list<std::string> header2 = {"a", "b"};
    std::vector<std::list<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"2", "x"});
    TableResult* tableResult2 = new TableResult(header2, rows2);

    std::list<std::string> resultHeader = {"s", "v", "a", "b"};
    std::vector<std::list<std::string>> resultRows;
    resultRows.push_back({"1", "x", "1", "x"});
    resultRows.push_back({"1", "x", "2", "x"});
    resultRows.push_back({"1", "y", "1", "x"});
    resultRows.push_back({"1", "y", "2", "x"});
    TableResult* expectedResult = new TableResult(resultHeader, resultRows);

    requireTrue(*expectedResult == *(Result::join(tableResult1, tableResult2)));
}

TEST_CASE("Test join: no common columns, 1x2") {
    std::list<std::string> header1 = {"s"};
    std::vector<std::list<std::string>> rows1;
    rows1.push_back({"1"});
    rows1.push_back({"2"});
    TableResult* tableResult1 = new TableResult(header1, rows1);

    std::list<std::string> header2 = {"a", "b"};
    std::vector<std::list<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"2", "x"});
    TableResult* tableResult2 = new TableResult(header2, rows2);

    std::list<std::string> resultHeader = {"s", "a", "b"};
    std::vector<std::list<std::string>> resultRows;
    resultRows.push_back({"1", "1", "x"});
    resultRows.push_back({"1", "2", "x"});
    resultRows.push_back({"2", "1", "x"});
    resultRows.push_back({"2", "2", "x"});
    TableResult* expectedResult = new TableResult(resultHeader, resultRows);

    requireTrue(*expectedResult == *(Result::join(tableResult1, tableResult2)));
}

TEST_CASE("Test join: no common columns, 3x2") {
    std::list<std::string> header1 = {"s", "a", "y"};
    std::vector<std::list<std::string>> rows1;
    rows1.push_back({"1", "1", "4"});
    rows1.push_back({"2", "1", "4"});
    TableResult* tableResult1 = new TableResult(header1, rows1);

    std::list<std::string> header2 = {"a", "b"};
    std::vector<std::list<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"2", "x"});
    TableResult* tableResult2 = new TableResult(header2, rows2);

    std::list<std::string> resultHeader = {"a", "s", "y", "b"};
    std::vector<std::list<std::string>> resultRows;
    resultRows.push_back({"1", "1", "4", "x"});
    resultRows.push_back({"1", "2", "4", "x"});
    TableResult* expectedResult = new TableResult(resultHeader, resultRows);

    requireTrue(*expectedResult == *(Result::join(tableResult1, tableResult2)));
}

TEST_CASE("Test output") {
    std::list<std::string> result;
    std::list<std::string> header1 = {"s", "a", "y"};
    std::vector<std::list<std::string>> rows1;
    rows1.push_back({"1", "1", "4"});
    rows1.push_back({"2", "1", "4"});
    TableResult* tableResult1 = new TableResult(header1, rows1);

    std::list<std::string> header2 = {"a", "b"};
    std::vector<std::list<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"2", "x"});
    TableResult* tableResult2 = new TableResult(header2, rows2);

    Result* resultTable = Result::join(tableResult1, tableResult2);
    std::string col = "a";
    resultTable->output(result, col);
    std::list<std::string> expectedResult;
    expectedResult.push_back("1");
    requireEqual(expectedResult, result);
}

TEST_CASE("Test output selected does not match") {
    std::list<std::string> result;
    BoolResult* boolResult = new BoolResult(false);

    std::list<std::string> header2 = {"a", "b"};
    std::vector<std::list<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"2", "x"});
    TableResult* tableResult2 = new TableResult(header2, rows2);

    Result* resultTable = Result::join(boolResult, tableResult2);
    std::string col = "";
    resultTable->output(result, col);

    requireTrue(result.size() == 1);
}
