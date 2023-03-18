#pragma once

#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <iterator>
#include <unordered_set>

#include "Result.h"
#include "SelectResult.h"

// n-col result
class TableResult : public Result {
 public:
    std::list<std::string> idents;  // eg list<"r", "s1">
    std::vector<std::list<std::string>> rows;  // eg vector<list<1,1>, list<3,2>, ...>

    // general constructor for n-cols
    TableResult(std::list<std::string> &_idents,
                const std::vector<std::list<std::string>> &_rows);

    // constructor for empty table
    TableResult();

    // for 2 cols with STMT_ENT_SET
    TableResult(const std::string &ident1, const std::string &ident2,
                STMT_ENT_SET &set);

    // for 2 cols with STMT_STMT_SET
    TableResult(const std::string &ident1, const std::string &ident2,
                STMT_STMT_SET &set);

    // for 2 cols with ENT_ENT_SET
    TableResult(const std::string &ident1, const std::string &ident2,
                ENT_ENT_SET &set);

    // for 2 cols with vector<list<string>>
    TableResult(const std::string &ident1, const std::string &ident2,
                const std::vector<std::list<std::string>> &vec);

    // for 1 col with ENT_SET
    TableResult(const std::string &ident, ENT_SET &set);

    // for 1 col with STMT_SET
    TableResult(const std::string &ident, STMT_SET &set);

    // SelectResult
    explicit TableResult(SelectResult &selectResult);

    void output(std::list<std::string> &list) override;

    bool operator==(const Result &rhs) const;
};
