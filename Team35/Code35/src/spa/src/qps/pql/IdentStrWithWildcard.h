#pragma once
#include <string>
#include "Tok.h"

/*
represents string identifier
*/
class IdentStrWithWildcard : public Tok {
public:
    explicit IdentStrWithWildcard(std::string s);
    const std::string s;
    std::string str() const;
    bool operator==(const Tok& rhs) const;
};

