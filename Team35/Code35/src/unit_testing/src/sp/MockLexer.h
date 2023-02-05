#pragma once

#include <vector>

#include "commons/token/Token.h"

class MockLexer {
 public:
    explicit MockLexer(const std::vector<Token>& tokens);
    Token scan();
private:
    std::vector<Token> tokenLst_;
};
