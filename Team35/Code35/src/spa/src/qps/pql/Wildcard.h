#pragma once
#include "PQLToken.h"
#include <string>

/*
 represented by "_"
*/
class Wildcard : public PQLToken {
 public:
    Wildcard();
    std::string str() const;
    bool operator==(const PQLToken& rhs) const;
};
