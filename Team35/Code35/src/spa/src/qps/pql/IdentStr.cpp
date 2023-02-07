#include "IdentStr.h"

IdentStr::IdentStr(std::string s) : Tok(IDENT), s(s) {}

std::string IdentStr::str() const {
  return s;
}
