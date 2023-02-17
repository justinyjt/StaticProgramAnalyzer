#include "catch.hpp"
#include "../TestHelper.h"
#include "qps/result/Result.h"
#include "qps/result/ResultsJoiner.h"

TEST_CASE("Test merge") {
    // stmt s; variable v; Select v such that Modifies(s,v) pattern a(v, _)
    ResultsJoiner *resultsJoiner;

    std::vector<std::string> header1 = {"s", "v"};
    std::vector<std::vector<std::string>> rows1;
    rows1.push_back({"1", "x"});
    rows1.push_back({"1", "y"});
    TableResults* tableResult1 = new TableResults(header1, rows1);

    std::vector<std::string> header2 = {"a", "v"};
    std::vector<std::vector<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"2", "x"});
    rows2.push_back({"1", "z"});
    TableResults* tableResult2 = new TableResults(header2, rows2);

    std::vector<std::string> resultHeader = {"v", "s", "a"};
    std::vector<std::vector<std::string>> resultRows;
    resultRows.push_back({"x", "1", "1"});
    resultRows.push_back({"x", "1", "2"});
    TableResults* expectedResult = new TableResults(resultHeader, resultRows);

    Results* results = resultsJoiner->join(*tableResult1, *tableResult2);

    requireTrue(*expectedResult == *results);
}

TEST_CASE("Test merge1") {
    // stmt s; variable v; Select v such that Modifies(s,v) pattern a(v, _)
    ResultsJoiner *resultsJoiner;

    BoolResults* boolResults = new BoolResults(true);

    std::vector<std::string> header2 = {"a", "v"};
    std::vector<std::vector<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"1", "z"});
    TableResults* tableResult2 = new TableResults(header2, rows2);

    std::vector<std::string> resultHeader = {"a", "v"};
    std::vector<std::vector<std::string>> resultRows;
    resultRows.push_back({"1", "x"});
    resultRows.push_back({"1", "z"});
    TableResults* expectedResult = new TableResults(resultHeader, resultRows);

    Results* results = resultsJoiner->join(*boolResults, *tableResult2);

    requireTrue(*expectedResult == *results);
}

TEST_CASE("Test merge2") {
    // stmt s; variable v; Select v such that Modifies(s,v) pattern a(v, _)
    ResultsJoiner *resultsJoiner;

    BoolResults* boolResults = new BoolResults(false);

    std::vector<std::string> header2 = {"a", "v"};
    std::vector<std::vector<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"1", "z"});
    TableResults* tableResult2 = new TableResults(header2, rows2);

    BoolResults* expectedResult = new BoolResults(false);

    Results* results = resultsJoiner->join(*boolResults, *tableResult2);

    requireTrue(*expectedResult == *results);
}


TEST_CASE("Test merge4") {
    // stmt s; variable v; Select v such that Modifies(s,v) pattern a(v, _)
    ResultsJoiner *resultsJoiner;

    std::vector<std::string> header1 = {"s", "v", "c"};
    std::vector<std::vector<std::string>> rows1;
    rows1.push_back({"1", "x", "1"});
    rows1.push_back({"1", "y", "5"});
    TableResults* tableResult1 = new TableResults(header1, rows1);

    std::vector<std::string> header2 = {"a", "v", "c"};
    std::vector<std::vector<std::string>> rows2;
    rows2.push_back({"1", "x", "2"});
    rows2.push_back({"2", "x", "2"});
    rows2.push_back({"1", "z", "3"});
    TableResults* tableResult2 = new TableResults(header2, rows2);

    std::vector<std::string> resultHeader = {"v", "c", "s", "a"};
    std::vector<std::vector<std::string>> resultRows;
    TableResults* expectedResult = new TableResults(resultHeader, resultRows);

    Results* results = resultsJoiner->join(*tableResult1, *tableResult2);

    requireTrue(*expectedResult == *results);
}

TEST_CASE("Test merge5") {
    // stmt s; variable v; Select v such that Modifies(s,v) pattern a(v, _)
    ResultsJoiner *resultsJoiner;

    std::vector<std::string> header1 = {"s", "v"};
    std::vector<std::vector<std::string>> rows1;
    rows1.push_back({"1", "x"});
    rows1.push_back({"1", "y"});
    TableResults *tableResult1 = new TableResults(header1, rows1);

    std::vector<std::string> header2 = {"a", "b"};
    std::vector<std::vector<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"2", "x"});
    TableResults *tableResult2 = new TableResults(header2, rows2);

    std::vector<std::string> resultHeader = {"s", "v", "a", "b"};
    std::vector<std::vector<std::string>> resultRows;
    resultRows.push_back({"1", "x", "1", "x"});
    resultRows.push_back({"1", "x", "2", "x"});
    resultRows.push_back({"1", "y", "1", "x"});
    resultRows.push_back({"1", "y", "2", "x"});
    TableResults *expectedResult = new TableResults(resultHeader, resultRows);

    Results *results = resultsJoiner->join(*tableResult1, *tableResult2);

    requireTrue(*expectedResult == *results);
}


TEST_CASE("Test merge6") {
    // stmt s; variable v; Select v such that Modifies(s,v) pattern a(v, _)
    ResultsJoiner *resultsJoiner;

    std::vector<std::string> header1 = {"s"};
    std::vector<std::vector<std::string>> rows1;
    rows1.push_back({"1"});
    rows1.push_back({"2"});
    TableResults *tableResult1 = new TableResults(header1, rows1);

    std::vector<std::string> header2 = {"a", "b"};
    std::vector<std::vector<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"2", "x"});
    TableResults *tableResult2 = new TableResults(header2, rows2);

    std::vector<std::string> resultHeader = {"s", "a", "b"};
    std::vector<std::vector<std::string>> resultRows;
    resultRows.push_back({"1", "1", "x"});
    resultRows.push_back({"1", "2", "x"});
    resultRows.push_back({"2", "1", "x"});
    resultRows.push_back({"2", "2", "x"});
    TableResults *expectedResult = new TableResults(resultHeader, resultRows);

    Results *results = resultsJoiner->join(*tableResult1, *tableResult2);

    requireTrue(*expectedResult == *results);
}

TEST_CASE("Test merge7") {
    // stmt s; variable v; Select v such that Modifies(s,v) pattern a(v, _)
    ResultsJoiner *resultsJoiner;

    std::vector<std::string> header1 = {"s", "a", "y"};
    std::vector<std::vector<std::string>> rows1;
    rows1.push_back({"1", "1", "4"});
    rows1.push_back({"2", "1", "4"});
    TableResults *tableResult1 = new TableResults(header1, rows1);

    std::vector<std::string> header2 = {"a", "b"};
    std::vector<std::vector<std::string>> rows2;
    rows2.push_back({"1", "x"});
    rows2.push_back({"2", "x"});
    TableResults *tableResult2 = new TableResults(header2, rows2);

    std::vector<std::string> resultHeader = {"a", "s", "y", "b"};
    std::vector<std::vector<std::string>> resultRows;
    resultRows.push_back({"1", "1", "4", "x"});
    resultRows.push_back({"1", "2", "4", "x"});
    TableResults *expectedResult = new TableResults(resultHeader, resultRows);

    Results *results = resultsJoiner->join(*tableResult1, *tableResult2);

    requireTrue(*expectedResult == *results);
}