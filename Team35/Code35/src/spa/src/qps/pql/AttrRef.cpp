#include <utility>

#include "AttrRef.h"

AttrRef::AttrRef(DesignEntity de, std::string ident, std::string ref) : Synonym(de, std::move(ident)), ref(ref) {}

bool AttrRef::operator==(const PQLToken &rhs) const {
    const AttrRef* p_rhs = dynamic_cast<const AttrRef*>(&rhs);
    return p_rhs != nullptr && de == p_rhs->de && ident == p_rhs->ident && ref == p_rhs->ref;
}

std::string AttrRef::str() const {
    return ident;
}
