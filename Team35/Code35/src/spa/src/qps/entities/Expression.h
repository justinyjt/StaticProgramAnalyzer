#pragma once

#include <string>
#include "Term.h"

class Expression : public Term {
 public:
    explicit Expression(std::string expression);

    std::string getExpression() const;
 private:
    std::string expression;
};
