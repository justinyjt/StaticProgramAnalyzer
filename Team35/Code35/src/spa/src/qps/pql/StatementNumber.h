#pragma once
#include "Tok.h"

class StatementNumber : public Tok {
public:
    StatementNumber(std::string value) : Tok::Tok(value,  STMT_NUM) {};
};
