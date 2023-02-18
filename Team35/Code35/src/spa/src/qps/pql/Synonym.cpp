#include "Synonym.h"

Synonym::Synonym(DesignEntity de, std::string ident) : PQLToken(Tag::SYNONYM), ident(ident), de(de) {}

bool Synonym::operator==(const PQLToken &rhs) const {
    const Synonym* p_rhs = dynamic_cast<const Synonym*>(&rhs);
    return p_rhs != nullptr && de == p_rhs->de && ident == p_rhs->ident;
}

std::string Synonym::str() const {
    return ident;
}
