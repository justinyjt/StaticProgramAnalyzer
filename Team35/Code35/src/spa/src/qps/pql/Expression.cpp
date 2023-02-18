#include "Expression.h"

Expression::Expression(std::string s) : PQLToken(Tag::EXPR), s(s) {}

std::string Expression::str() const {
    return s;
}

bool Expression::operator==(const PQLToken& rhs) const {
  const Expression* p_rhs = dynamic_cast<const Expression*>(&rhs);
  return p_rhs != nullptr && s == p_rhs->s;
}
