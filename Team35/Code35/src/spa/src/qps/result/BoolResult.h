#pragma once
#include <list>
#include <memory>
#include <string>

#include "Result.h"

// scalar result
class BoolResult : public Result {
 public:
    const bool b;

    explicit BoolResult(bool b);

    std::unique_ptr<Result> join(Result &rhs) override;

    void output(std::list<std::string> &list) const override;

    bool operator==(const Result &rhs) const;

    bool isNull() const override;
};
