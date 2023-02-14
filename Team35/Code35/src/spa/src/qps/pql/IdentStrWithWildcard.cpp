#include "IdentStrWithWildcard.h"

IdentStrWithWildcard::IdentStrWithWildcard(std::string s) : Tok(Tag::IDENT_WILDCARD), s(s) {}

std::string IdentStrWithWildcard::str() const {
    return s;
}

bool IdentStrWithWildcard::operator==(const Tok& rhs) const {
    const IdentStrWithWildcard* p_rhs = dynamic_cast<const IdentStrWithWildcard*>(&rhs);
    return p_rhs != NULL && Tok::equal(rhs) && s == p_rhs->s;
}
