#include "Tok.h"

Tok::Tok(Tag tag) : tag(tag) {}

bool Tok::equal(const Tok &rhs) const {
    return tag == rhs.tag;
}

bool Tok::operator!=(const Tok &rhs) const {
    return !(*this == rhs);
}
