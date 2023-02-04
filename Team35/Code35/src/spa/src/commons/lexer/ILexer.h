#pragma once

#include <string>

#include "commons/token/Token.h"

typedef std::string Source;

class ILexer {
 public:
    virtual Token Scan() = 0;
    virtual ~ILexer() = default;
};
