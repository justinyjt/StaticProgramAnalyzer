#pragma once
#include <string>
#include "PQLToken.h"

/*
represents string identifier
*/
class Ident : public PQLToken {
 public:
    explicit Ident(std::string s);
    const std::string s;
    std::string str() const override;
    bool operator==(const PQLToken& rhs) const override;
};
