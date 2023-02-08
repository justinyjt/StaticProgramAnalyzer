#pragma once

#include <memory>
#include "commons/lexer/ILexer.h"

class TokenValidator {
public:
    TokenValidator(std::unique_ptr<ILexer> &lexer);
    // peek, consume, etc
    std::unique_ptr<Token> getNextToken();
private:
    std::unique_ptr<ILexer> &lexer;
};

