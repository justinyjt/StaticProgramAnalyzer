#pragma once

#include <memory>
#include <string>

#include "commons/token/Token.h"

class ILexer {
 public:
    virtual std::unique_ptr<Token> scan() = 0;
    virtual void reset() = 0;
    virtual ~ILexer() = default;
};
