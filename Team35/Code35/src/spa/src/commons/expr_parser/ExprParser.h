#pragma once

#include <memory>

#include "ExprNode.h"
#include "sp/IParser.h"
#include "commons/token_scanner/SimpleTokenScanner.h"
#include "commons/types.h"


class ExprParser {
 public:
    explicit ExprParser(TokenScanner *scanner);

    ASSIGN_PAT_RIGHT parseExpr();

 private:
    TokenScanner *scanner_;

    ASSIGN_PAT_RIGHT parseTerm();

    ASSIGN_PAT_RIGHT parseFactor();

    ASSIGN_PAT_RIGHT parseName();

    ASSIGN_PAT_RIGHT parseInteger();
};
