#pragma once
#include <list>
#include <string>

#include "Result.h"
#include "TableResult.h"

// scalar result
class BoolResult : public Result {
 public:
    bool b;
    explicit BoolResult(bool b);
    std::unique_ptr<Result> getFinalResult(TableResult intermediateResultTable);
    std::unique_ptr<Result> join(Result &);
    void output(std::list<std::string> &list) override;
    bool operator==(const Result &rhs) const;
};
