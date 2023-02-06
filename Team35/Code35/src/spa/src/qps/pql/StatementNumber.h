#pragma once

#include <string>
#include "Tok.h"

class StatementNumber : public Tok {
 public:
    explicit StatementNumber(std::string value);
};
