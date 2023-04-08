#pragma once

#include <memory>

#include "TokenScanner.h"

class SimpleTokenScanner : public TokenScanner {
 public:
    explicit SimpleTokenScanner(std::unique_ptr<ILexer> lex);

    int isCondExprSeparatedByLogicalOperator() const;

    int isAssign() const;
};
