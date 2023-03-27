#pragma once
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Result.h"
#include "TableResult.h"

// n-col result
class SelectResult : public Result {
 public:
    std::vector<std::string> idents;  // eg list<"v", "s1">
    std::vector<TableResult> cols;  // eg vector<TableResult(v), TableResult(s1), ...>

    // general constructor for n-cols
    SelectResult(std::vector<std::string> &_idents, const std::vector<TableResult> &_cols);

    std::unique_ptr<Result> getColsCrossProduct();

    std::unique_ptr<Result> join(Result &rhs);

    void output(std::list<std::string> &list) override;

    bool operator==(const Result &rhs) const;
};
