#pragma once
#include "string"
#include "Tok.h"

/*
"<expr>, _<expr>_, _"
*/ 
class ExpressionStr : public Tok {
 public:
    explicit ExpressionStr(std::string str);
};
