#pragma once

#include "commons/token_scanner/TokenScanner.h"

class ExprValidator {
 public:
    explicit ExprValidator(TokenScanner &scanner);

    bool validateExpr();

 private:
    TokenScanner &scanner_;

    bool validateTerm();

    bool validateFactor();

    bool validateName();

    bool validateInt();
};
