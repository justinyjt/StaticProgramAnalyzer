#pragma once
#include <list>
#include <memory>
#include <string>

#include "Result.h"
#include "TableResult.h"

// scalar result
class BoolResult : public Result {
 public:
    bool b;

    bool isSelectBool;

    explicit BoolResult(bool b);

    explicit BoolResult(bool b, bool isSelectBool);

    std::unique_ptr<Result> join(Result &rhs) override;

    void output(std::list<std::string> &list) override;

    bool operator==(const Result &rhs) const;

    bool isNull() const override;
};
