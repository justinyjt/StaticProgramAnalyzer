#include "Character.h"

Character::Character(char ascii_lexeme, LineNumber line_number) : Token(std::string(1, ascii_lexeme),
                                                                        Tag::Ascii,
                                                                        line_number), ascii_lexeme_(ascii_lexeme) {}

Character::Character(char ascii_lexeme, Tag tag, LineNumber line_number) : Token(std::string(1, ascii_lexeme),
                                                                                 tag,
                                                                                 line_number),
                                                                           ascii_lexeme_(ascii_lexeme) {}

char Character::GetCharacter() {
    return ascii_lexeme_;
}
