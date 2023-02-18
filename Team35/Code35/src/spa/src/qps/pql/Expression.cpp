#include "Expression.h"

#include <utility>

Expression::Expression(std::string s, bool hasWildcard) : PQLToken(Tag::EXPR), s(s), hasWildcard(hasWildcard) {}

std::string Expression::str() const {
    return s;
}

bool Expression::operator==(const PQLToken& rhs) const {
    const auto* p_rhs = dynamic_cast<const Expression*>(&rhs);
    return p_rhs != nullptr && PQLToken::equal(rhs) && s == p_rhs->s && hasWildcard == p_rhs->hasWildcard;
}
