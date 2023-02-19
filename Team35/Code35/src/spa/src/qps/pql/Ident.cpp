#include "Ident.h"

#include <utility>

Ident::Ident(std::string s) : PQLToken(Tag::IDENT), s(s) {}

std::string Ident::str() const {
  return s;
}

bool Ident::operator==(const PQLToken& rhs) const {
  const Ident* p_rhs = dynamic_cast<const Ident*>(&rhs);
  return p_rhs != nullptr && s == p_rhs->s;
}
