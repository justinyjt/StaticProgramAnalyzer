#include "AttrRef.h"

#include <utility>

AttrRef::AttrRef(DesignEntity de, std::string ident, std::string ref) : Synonym(de, std::move(ident)),
                                                                        ref(std::move(ref)) {}

bool AttrRef::operator==(const PQLToken &rhs) const {
    const auto *p_rhs = dynamic_cast<const AttrRef *>(&rhs);
    return p_rhs != nullptr && de == p_rhs->de && ident == p_rhs->ident && ref == p_rhs->ref;
}

std::string AttrRef::str() const {
    return ident;
}
