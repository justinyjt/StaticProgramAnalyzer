#include "ExpressionStr.h"

ExpressionStr::ExpressionStr(std::string s) : Tok(Tag::EXPR), s(s) {}

std::string ExpressionStr::str() const {
    return s;
}

bool ExpressionStr::operator==(const Tok& rhs) const {
  const ExpressionStr* p_rhs = dynamic_cast<const ExpressionStr*>(&rhs);
  return p_rhs != NULL && Tok::equal(rhs) && s == p_rhs->s;
}
