#include "Integer.h"

Integer::Integer(int integer_lexeme, LineNumber line_number) : Token(std::to_string(integer_lexeme),
                                                                     Tag::Integer,
                                                                     line_number), integer_lexeme_(integer_lexeme) {}

Integer::Integer(int integer_lexeme, Tag tag, LineNumber line_number) : Token(std::to_string(integer_lexeme),
                                                                              tag,
                                                                              line_number),
                                                                        integer_lexeme_(integer_lexeme) {}

int Integer::GetInteger() {
    return integer_lexeme_;
}
