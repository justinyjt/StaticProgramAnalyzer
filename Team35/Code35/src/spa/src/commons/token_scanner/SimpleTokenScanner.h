#pragma once

#include "TokenScanner.h"

class SimpleTokenScanner : public TokenScanner {
 public:
    explicit SimpleTokenScanner(std::unique_ptr<ILexer> lex);
    explicit SimpleTokenScanner(TokenLst token_lst);

    int isName() const;

    int isCondExprSeparatedByLogicalOperator() const;
};
