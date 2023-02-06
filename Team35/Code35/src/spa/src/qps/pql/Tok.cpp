#include "Tok.h"

Tok::Tok(std::string value, Tag tag) : value(value), tag(tag) {}

bool Tok::operator==(const Tok &rhs) const {
    return this->tag == rhs.tag;
}

std::string Tok::getValue() {
    return value;
}
