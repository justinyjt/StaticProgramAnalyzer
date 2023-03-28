#pragma once
#include <list>
#include <memory>
#include <string>

#include "Result.h"

// scalar result
class IdentityResult : public Result {
 public:
    explicit IdentityResult();

    std::unique_ptr<Result> join(Result &rhs);

    void output(std::list<std::string> &list) override;
};
