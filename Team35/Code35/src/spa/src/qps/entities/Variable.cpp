#include "Variable.h"

#include <utility>

Variable::Variable(std::string variableName) : Term::Term(Term::Tag::Variable), variableName(std::move(variableName)) {}

std::string Variable::getVariableName() {
    return variableName;
}