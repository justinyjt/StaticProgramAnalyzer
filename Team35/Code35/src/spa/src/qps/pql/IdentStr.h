#pragma once
#include <string>
#include "Tok.h"

/*
represents string identifier
*/
class IdentStr : public Tok {
 public:
        explicit IdentStr(std::string s);
        const std::string s;
        std::string str() const;
        bool operator==(const Tok& rhs) const;
};
