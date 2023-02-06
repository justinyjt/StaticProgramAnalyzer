#pragma once
#include <string>
#include "Tok.h"

/*
represents string identifier
*/
class IdentStr : public Tok {
    IdentStr(std::string s) : Tok::Tok(s,  IDENT) {};
};
