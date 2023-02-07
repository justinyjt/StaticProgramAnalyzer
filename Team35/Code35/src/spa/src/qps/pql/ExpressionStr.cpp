#include "ExpressionStr.h"

ExpressionStr::ExpressionStr(std::string s) : Tok(EXPR), s(s) {}

std::string ExpressionStr::str() const {
    return s;
}