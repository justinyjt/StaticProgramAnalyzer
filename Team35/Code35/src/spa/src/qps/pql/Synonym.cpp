#include "Synonym.h"

Synonym::Synonym(DesignEntity de, std::string ident) : Tok(SYNONYM), ident(ident), de(de) {}

bool Synonym::operator==(const Synonym &rhs) const {
    return this->de == rhs.de && this->ident == rhs.ident;
}

std::string Synonym::str() const {
    return ident;
}
