#pragma once

#include <string>

#include "commons/token/Token.h"

class Integer : Token {
 public:
    Integer(int integer_lexeme, LineNumber line_number);
    Integer(int integer_lexeme, Tag tag, LineNumber line_number);
    int GetInteger();

 private:
    int integer_lexeme_;
};
