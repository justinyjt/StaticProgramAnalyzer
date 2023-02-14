#include "Tok.h"
#include "Wildcard.h"

Wildcard::Wildcard() : Tok(Tag::WILDCARD) {}

std::string Wildcard::str() const {
  return "_";
}

bool Wildcard::operator==(const Tok& rhs) const {
  const Wildcard* p_rhs = dynamic_cast<const Wildcard*>(&rhs);
  return p_rhs != NULL && Tok::equal(rhs);
}
