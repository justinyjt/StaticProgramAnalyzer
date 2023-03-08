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

    std::unique_ptr<ExprNode> parseTerm();

    std::unique_ptr<ExprNode> parseFactor();

    std::unique_ptr<ExprNode> parseName();

    std::unique_ptr<ExprNode> parseInteger();
};
