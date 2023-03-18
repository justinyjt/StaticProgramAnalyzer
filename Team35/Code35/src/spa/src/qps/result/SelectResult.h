#pragma once
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <iterator>
#include <unordered_set>

#include "Result.h"

// n-col result
class SelectResult : public Result {
public:
    std::list<std::string> idents;  // eg list<"v", "s1">
    std::vector<std::list<std::string>> rows;  // eg vector<list<"x",1>, list<"y",2>, ...>
    std::string selected;

    // general constructor for n-cols
    SelectResult(std::list<std::string> &_idents,
                const std::vector<std::list<std::string>> &_rows, std::string selected);

    // constructor for empty table
    SelectResult();

    // for 2 cols with STMT_ENT_SET
    SelectResult(const std::string &ident1, const std::string &ident2,
                STMT_ENT_SET &set, std::string selected);

    // for 2 cols with STMT_STMT_SET
    SelectResult(const std::string &ident1, const std::string &ident2,
                STMT_STMT_SET &set, std::string selected);

    // for 2 cols with ENT_ENT_SET
    SelectResult(const std::string &ident1, const std::string &ident2,
                ENT_ENT_SET &set, std::string selected);

    // for 2 cols with vector<list<string>>
    SelectResult(const std::string &ident1, const std::string &ident2,
                const std::vector<std::list<std::string>> &vec, std::string selected);

    // for 1 col with ENT_SET
    SelectResult(const std::string &ident, ENT_SET &set, std::string selected);

    // for 1 col with STMT_SET
    SelectResult(const std::string &ident, STMT_SET &set, std::string selected);

    void output(std::list<std::string> &list) override;

    bool operator==(const Result &rhs) const;
};
