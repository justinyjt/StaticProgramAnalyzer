#pragma once
#include <string>
#include "PQLToken.h"

/*
represents string identifier
*/
class IdentStr : public PQLToken {
 public:
        explicit IdentStr(std::string s);
        const std::string s;
        std::string str() const;
        bool operator==(const PQLToken& rhs) const;
};
