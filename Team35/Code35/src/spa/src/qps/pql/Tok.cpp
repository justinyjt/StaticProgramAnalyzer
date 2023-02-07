#include "Tok.h"

Tok::Tok(Tag tag) : tag(tag) {}

bool Tok::operator==(const Tok &rhs) const {
    return this->tag == rhs.tag;
}