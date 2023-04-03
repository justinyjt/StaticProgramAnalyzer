#include "Character.h"

Character::Character(char ascii_lexeme) : Token::Token(std::string(1, ascii_lexeme), Token::Tag::Ascii),
                                          ascii_lexeme_(ascii_lexeme) {}

Character::Character(char ascii_lexeme, Token::Tag tag) : Token::Token(std::string(1, ascii_lexeme), tag),
                                                          ascii_lexeme_(ascii_lexeme) {}

char Character::getCharacter() const {
    return ascii_lexeme_;
}
