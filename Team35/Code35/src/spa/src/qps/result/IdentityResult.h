#pragma once
#include <list>
#include <string>

#include "Result.h"
#include "TableResult.h"
#include "BoolResult.h"

// Identity result
class IdentityResult : public Result {
 public:
    std::unique_ptr<Result> join(Result &rhs);
    void output(std::list<std::string> &list) override;
};
