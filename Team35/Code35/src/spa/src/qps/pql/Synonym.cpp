#include "Synonym.h"

Synonym::Synonym(DesignEntity de, std::string ident) : Tok::Tok(ident, SYNONYM), de(de) {}

bool Synonym::operator==(const Synonym &rhs) const {
    return this->de == rhs.de && this->value == rhs.value;
}
