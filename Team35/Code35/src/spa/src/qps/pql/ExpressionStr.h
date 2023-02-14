#pragma once
#include "string"
#include "PQLToken.h"

/*
"<expr>, _<expr>_, _"
*/ 
class ExpressionStr : public PQLToken {
 public:
    explicit ExpressionStr(std::string str);
    const std::string s;
    std::string str() const;
    bool operator==(const PQLToken& rhs) const;
};
