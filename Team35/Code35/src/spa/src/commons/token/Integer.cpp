#include "Integer.h"

Integer::Integer(int integer_lexeme) : Token::Token(std::to_string(integer_lexeme), Token::Tag::Integer),
                                       integer_lexeme_(integer_lexeme) {}

Integer::Integer(int integer_lexeme, Token::Tag tag) : Token::Token(std::to_string(integer_lexeme), tag),
                                                       integer_lexeme_(integer_lexeme) {}

Integer::Integer(int integer_lexeme, LineNumber line_number) : Token::Token(std::to_string(integer_lexeme),
                                                                            Token::Tag::Integer,
                                                                            line_number),
                                                               integer_lexeme_(integer_lexeme) {}

Integer::Integer(int integer_lexeme, Token::Tag tag, LineNumber line_number) : Token::Token(std::to_string(
                                                                                                integer_lexeme),
                                                                                            tag,
                                                                                            line_number),
                                                                               integer_lexeme_(integer_lexeme) {}

int Integer::GetInteger() const {
    return integer_lexeme_;
}
