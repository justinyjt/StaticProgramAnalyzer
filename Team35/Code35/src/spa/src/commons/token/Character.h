#pragma once

#include <string>

#include "commons/token/Token.h"

class Character : Token {
 public:
    Character(char ascii_lexeme, LineNumber line_number);
    Character(char ascii_lexeme, Tag tag, LineNumber line_number);
    char GetCharacter();

 private:
    char ascii_lexeme_;
};
