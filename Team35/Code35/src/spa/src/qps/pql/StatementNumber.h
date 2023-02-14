#pragma once

#include <string>
#include "Tok.h"

class StatementNumber : public Tok {
 public:
    explicit StatementNumber(int n);
    int n;
    std::string str() const;
    bool operator==(const Tok& rhs) const;
};
