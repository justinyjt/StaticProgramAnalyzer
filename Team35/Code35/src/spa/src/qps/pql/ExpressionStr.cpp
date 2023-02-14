#include "ExpressionStr.h"

ExpressionStr::ExpressionStr(std::string s) : PQLToken(Tag::EXPR), s(s) {}

std::string ExpressionStr::str() const {
    return s;
}

bool ExpressionStr::operator==(const PQLToken& rhs) const {
  const ExpressionStr* p_rhs = dynamic_cast<const ExpressionStr*>(&rhs);
  return p_rhs != NULL && PQLToken::equal(rhs) && s == p_rhs->s;
}
