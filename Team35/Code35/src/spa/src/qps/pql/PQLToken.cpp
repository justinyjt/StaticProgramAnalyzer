#include "PQLToken.h"

PQLToken::PQLToken(Tag tag) : tag(tag) {}

bool PQLToken::equal(const PQLToken &rhs) const {
    return tag == rhs.tag;
}

bool PQLToken::operator!=(const PQLToken &rhs) const {
    return !(*this == rhs);
}
