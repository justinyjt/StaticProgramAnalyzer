#include "IdentStr.h"

IdentStr::IdentStr(std::string s) : PQLToken(Tag::IDENT), s(s) {}

std::string IdentStr::str() const {
  return s;
}

bool IdentStr::operator==(const PQLToken& rhs) const {
  const IdentStr* p_rhs = dynamic_cast<const IdentStr*>(&rhs);
  return p_rhs != NULL && PQLToken::equal(rhs) && s == p_rhs->s;
}
