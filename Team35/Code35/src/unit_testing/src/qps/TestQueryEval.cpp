/**********************************************************************
#include "catch.hpp"
#include "../TestHelper.h"
#include "qps/QueryEvaluator.h"
#include "qps/clause/TwoArgClause/StmtEntClause.h"
#include "qps/clause/TwoArgClause/Pattern.h"
#include "qps/clause/Clause.h"
#include "qps/pql/StatementNumber.h"
#include "qps/pql/Expression.h"
#include "qps/pql/Wildcard.h"
#include "pkb/PKBReader.h"
#include "pkb/PKB.h"

PKB testPKB;

class PKBReaderStub : public PKBReader {
    public:
    PKBReaderStub() : PKBReader(testPKB) {};

    // x = x + 1
    // Select v such that Modifies(1, v)
    ENT_SET getRelationship(StmtNameRelationship tableType, STMT_NUM stmt) const {
        std::unordered_set<std::string> s;
        s.insert("x");
        return s;
    };

    // x = x + 1
    // Select a pattern a("_", "x + 1")
    STMT_SET getStmtWithExactPatternMatch(std::string &pattern) const {
        std::unordered_set<int> s;
        s.insert(1);
        return s;
    };
};

// x = x + 1
// variable v; Select v such that Modifies(1,v)";
TEST_CASE("Query Evaluator Modifies") {


    PKBReaderStub* db = new PKBReaderStub();
    QueryEvaluator qe(db);
    std::list<std::string> resultList;

    // Perform evaluation
    Synonym syn(Synonym::DesignEntity::VARIABLE, "v");
    std::vector<std::unique_ptr<Clause>> clauses;
    clauses.push_back(std::make_unique<ModifiesS>(StatementNumber("1"), syn));
    std::unique_ptr<Result> evalResult = qe.evaluate(syn, clauses);
    evalResult->output(resultList);
    requireEqual(resultList.front(), std::string("x"));
}

// x = x + 1
// assign a, Select a pattern a (_, \"x + 1\")
TEST_CASE("Query Evaluator Pattern") {

    std::string query = "";

    PKBReaderStub* db = new PKBReaderStub();
    QueryEvaluator qe(dynamic_cast<PKBReader*>(db));
    std::list<std::string> resultList;

    // Perform evaluation
    Synonym syn(Synonym::DesignEntity::ASSIGN, "a");
    std::vector<std::unique_ptr<Clause>> clauses;
    clauses.push_back(std::make_unique<Pattern>(Wildcard(), Expression("x + 1")));
    std::unique_ptr<Result> evalResult = qe.evaluate(syn, clauses);
    evalResult->output(resultList);
    requireEqual(resultList.front(), std::string("1"));
}
**********************************************************************/
