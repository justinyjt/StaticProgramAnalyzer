#pragma once

#include <cstdint>
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
    std::vector<std::string> idents;
    std::vector<TableResult> cols;

    // general constructor for n-cols
    SelectResult(std::vector<std::string> &_idents, const std::vector<TableResult> &_cols);

    std::unique_ptr<Result> join(Result &rhs) override;

    void output(std::list<std::string> &list) const override;

    bool operator==(const Result &rhs) const;

    bool isNull() const override;

 private:
    [[nodiscard]] std::vector<uint32_t> getOutputOrder(const TableResult &intermediateRes) const;

    std::unique_ptr<TableResult> getResultForOutput(std::unique_ptr<Result> finalRes);

    std::unique_ptr<Result> getColsCrossProduct();
};
