#pragma once
#include "PQLToken.h"
#include <string>

/*
 represented by "_"
*/
class Wildcard : public PQLToken {
 public:
    Wildcard();
    std::string str() const override;
    bool operator==(const PQLToken& rhs) const override;
};
