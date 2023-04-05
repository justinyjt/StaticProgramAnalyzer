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
    // constructor for empty table
    TableResult();

    // general constructor for n-cols
    TableResult(const std::vector<std::string> &idents, const std::vector<std::vector<std::string>> &rows);

    // constructor for SelectResult Output
    TableResult(const TableResult &tableRes, const std::vector<int> &order);

    // for 2 cols with STMT_ENT_SET
    TableResult(const std::string &ident1, const std::string &ident2, const STMT_ENT_SET &set);

    // for 2 cols with STMT_STMT_SET
    TableResult(const std::string &ident1, const std::string &ident2, const STMT_STMT_SET &set);

    // for 2 cols with ENT_ENT_SET
    TableResult(const std::string &ident1, const std::string &ident2, const ENT_ENT_SET &set);

    // for 2 cols with vector<vector<string>>
    TableResult(const std::string &ident1, const std::string &ident2,
                const std::vector<std::vector<std::string>> &vec);

    // for 1 col with ENT_SET
    TableResult(const std::string &ident, const ENT_SET &set);

    // for 1 col with STMT_SET
    TableResult(const std::string &ident, const STMT_SET &set);

    // 2 col with ENT_SET
    TableResult(const std::string &ident, const std::string &ident2, const std::vector<ENT_NAME> &set);

    std::unique_ptr<TableResult> projectColumns(const std::unordered_set<std::string> &projectedColumns) const;

    std::unique_ptr<Result> join(Result &rhs) override;

    void output(std::list<std::string> &list) const override;

    bool operator==(const Result &rhs) const;

    bool isNull() const override;

    const std::vector<std::string> &getIdents() const;

 private:
    std::vector<std::string> idents_;
    std::vector<std::vector<std::string>> rows_;
    std::optional<std::vector<int>> order_;  // order in which to output result
};
