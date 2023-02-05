#include "Expression.h"
#include <utility>

Expression::Expression(std::string expression) : Term::Term(Term::Tag::Expression), expression(std::move(expression)) {}

std::string Expression::getExpression() const {
    return expression;
}
