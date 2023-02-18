#include "Synonym.h"

#include <utility>

Synonym::Synonym(DesignEntity de, std::string ident) : PQLToken(Tag::SYNONYM), ident(std::move(ident)), de(de) {}

bool Synonym::operator==(const PQLToken &rhs) const {
    const auto* p_rhs = dynamic_cast<const Synonym*>(&rhs);
    return p_rhs != nullptr && PQLToken::equal(rhs) && de == p_rhs->de && ident == p_rhs->ident;
}

std::string Synonym::str() const {
    return ident;
}
