#pragma once
#include "string"
#include "Tok.h"

/*
"<expr>, _<expr>_, _"
*/ 
class ExpressionStr : public Tok {
public:
    ExpressionStr(std::string str) : Tok::Tok(str,  EXPR) {};
};