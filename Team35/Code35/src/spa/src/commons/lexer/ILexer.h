#pragma once

#include "commons/token/Token.h"

class ILexer {
 public:
    virtual Token Scan() = 0;
};
