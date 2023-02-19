#include "PQLToken.h"
#include "Wildcard.h"

Wildcard::Wildcard() : PQLToken(Tag::WILDCARD) {}

std::string Wildcard::str() const {
    return "_";
}

bool Wildcard::operator==(const PQLToken &rhs) const {
    const auto *p_rhs = dynamic_cast<const Wildcard *>(&rhs);
    return p_rhs != nullptr;
}
