#pragma once

#include "commons/token/Token.h"

class Keyword : Token {
 public:
    Keyword(Lexeme lexeme, LineNumber line_number);
    Keyword(Lexeme lexeme, Tag tag, LineNumber line_number);
};
