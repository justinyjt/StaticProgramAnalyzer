#pragma once
#include <list>
#include <memory>
#include <string>

#include "Result.h"

// scalar result
class IdentityResult : public Result {
 public:
    IdentityResult();

    std::unique_ptr<Result> join(Result &rhs) override;

    void output(std::list<std::string> &list) override;

    bool empty() const override;
};
