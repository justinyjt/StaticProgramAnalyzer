#include "Character.h"

Character::Character(char ascii_lexeme) : Token::Token(std::string(1, ascii_lexeme), Token::Tag::Ascii),
                                          ascii_lexeme_(ascii_lexeme) {}
Character::Character(char ascii_lexeme, Token::Tag tag) : Token::Token(std::string(1, ascii_lexeme), tag),
                                                          ascii_lexeme_(ascii_lexeme) {}

Character::Character(char ascii_lexeme, LineNumber line_number) : Token::Token(std::string(1, ascii_lexeme),
                                                                               Token::Tag::Ascii,
                                                                               line_number),
                                                                  ascii_lexeme_(ascii_lexeme) {}

Character::Character(char ascii_lexeme, Tag tag, LineNumber line_number) : Token::Token(std::string(1, ascii_lexeme),
                                                                                        tag,
                                                                                        line_number),
                                                                           ascii_lexeme_(ascii_lexeme) {}

Character::Character(const Character &character, LineNumber line_number) : Token(character, line_number),
                                                                           ascii_lexeme_(character.ascii_lexeme_) {}

char Character::GetCharacter() {
    return ascii_lexeme_;
}
