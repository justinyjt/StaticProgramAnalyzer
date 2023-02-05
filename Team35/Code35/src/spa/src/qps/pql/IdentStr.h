#pragma once
#include <string>
#include "Tok.h"

/*
represents string identifier
*/
class IdentStr : public Tok {
  std::string s;
  IdentStr(std::string s) : s(s), Tok(IDENT) {};
};
