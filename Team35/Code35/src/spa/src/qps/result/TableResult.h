#pragma once

#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <iterator>
#include <unordered_set>
#include <set>
#include <memory>

#include "Result.h"

// n-col result
class TableResult : public Result {
 public:
    std::vector<std::string> idents;  // eg list<"r", "s1">
    std::vector<std::vector<std::string>> rows;  // eg vector<list<1,1>, list<3,2>, ...>
    std::optional<std::vector<int>> order;  // Order in which to output result

    // general constructor for n-cols
    TableResult(const std::vector<std::string> &_idents,
                const std::vector<std::vector<std::string>> &_rows);

    // constructor for SelectResult Output
    TableResult(const std::vector<std::string> &_idents,
                const std::vector<std::vector<std::string>> &_rows,
                const std::vector<int> &_order);

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

    // for 2 cols with vector<vector<string>>
    TableResult(const std::string &ident1, const std::string &ident2,
                const std::vector<std::vector<std::string>> &vec);

    // for 1 col with ENT_SET
    TableResult(const std::string &ident, ENT_SET &set);

    // for 1 col with STMT_SET
    TableResult(const std::string &ident, STMT_SET &set);

    // 2 col with ENT_SET
    TableResult(const std::string &ident, const std::string &ident2, const std::vector<ENT_NAME> &set);

    std::unique_ptr<TableResult> projectColumns(const std::unordered_set<std::string> &projectedColumns) const;

    std::unique_ptr<Result> join(Result &rhs) override;

    void output(std::list<std::string> &list) override;

    bool operator==(const Result &rhs) const;

    bool empty() const override;
};
