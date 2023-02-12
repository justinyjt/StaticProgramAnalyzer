#pragma once
#include "Tok.h"
#include <string>

/*
 represented by "_"
*/
class Wildcard : public Tok {
 public:
    Wildcard();
    std::string str() const;
    bool operator==(const Tok& rhs) const;
};
