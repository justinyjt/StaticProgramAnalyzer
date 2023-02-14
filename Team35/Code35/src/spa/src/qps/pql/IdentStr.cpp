#include "IdentStr.h"

IdentStr::IdentStr(std::string s) : Tok(Tag::IDENT), s(s) {}

std::string IdentStr::str() const {
  return s;
}

bool IdentStr::operator==(const Tok& rhs) const {
  const IdentStr* p_rhs = dynamic_cast<const IdentStr*>(&rhs);
  return p_rhs != NULL && Tok::equal(rhs) && s == p_rhs->s;
}
