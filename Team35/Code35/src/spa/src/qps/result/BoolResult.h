#pragma once
#include <list>
#include <string>
#include "Result.h"

// scalar result
class BoolResult : public Result {
 public:
    bool b;
    explicit BoolResult(bool b);

    void output(std::list<std::string>& list, std::string& selected) override;
    bool operator==(const Result& rhs) const;
};
