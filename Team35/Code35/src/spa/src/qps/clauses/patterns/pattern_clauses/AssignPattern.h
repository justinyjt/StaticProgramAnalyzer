#pragma once

#include <string>
#include "Pattern.h"

class AssignPattern : public Pattern {
 public:
    AssignPattern(Term left, Term right);
};
