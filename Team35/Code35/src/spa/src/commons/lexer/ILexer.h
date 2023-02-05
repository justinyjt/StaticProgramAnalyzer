#pragma once

#include <string>

#include "commons/token/Token.h"

class ILexer {
 public:
    virtual Token scan() = 0;
    virtual ~ILexer() = default;
};
