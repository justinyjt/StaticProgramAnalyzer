#pragma once

#include <string>

#include "Token.h"

class Character : public Token {
 public:
    explicit Character(char ascii_lexeme);
    Character(char ascii_lexeme, Tag tag);
    char getCharacter() const;

 private:
    char ascii_lexeme_;
};
