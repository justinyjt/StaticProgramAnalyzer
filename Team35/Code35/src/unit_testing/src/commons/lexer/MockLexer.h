#pragma once

#include <memory>
#include <vector>

#include "commons/token/Token.h"
#include "commons/lexer/ILexer.h"

class MockLexer : public ILexer {
 public:
    explicit MockLexer(const std::vector<Token> &tokens);
    std::unique_ptr<Token> scan() override;
    void reset() override;
 private:
    std::vector<Token> tokenLst_;
};
