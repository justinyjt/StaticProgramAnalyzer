#include "Expression.h"

#include <utility>

Expression::Expression(ASSIGN_PAT_RIGHT exprNode, bool hasWildcard) :
        PQLToken(Tag::EXPR), exprNode(exprNode), hasWildcard(hasWildcard) {}

std::string Expression::str() const {
    return exprNode->toString();
}

bool Expression::operator==(const PQLToken& rhs) const {
  const Expression* p_rhs = dynamic_cast<const Expression*>(&rhs);
  return p_rhs != nullptr && *exprNode == *(p_rhs->exprNode) && hasWildcard == p_rhs->hasWildcard;
}
