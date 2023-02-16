#pragma once

#include <string>
#include "PQLToken.h"

class StatementNumber : public PQLToken {
 public:
    explicit StatementNumber(int n);
    int n;
    std::string str() const;
    bool operator==(const PQLToken& rhs) const;
};
