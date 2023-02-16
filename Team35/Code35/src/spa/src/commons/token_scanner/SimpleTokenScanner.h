#pragma once

#include "TokenScanner.h"

class SimpleTokenScanner : TokenScanner {
    explicit SimpleTokenScanner(std::unique_ptr<ILexer> lex);

    int isName() const;

    int isCondExprSeparatedByLogicalOperator() const;
};
