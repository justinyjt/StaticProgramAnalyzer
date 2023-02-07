#include "Tok.h"
#include "Wildcard.h"

Wildcard::Wildcard() : Tok(WILDCARD) {}

std::string Wildcard::str() const {
  return "_";
}
