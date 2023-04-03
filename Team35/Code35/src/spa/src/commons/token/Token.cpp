#include "Token.h"

#include <utility>

Token::Token(Token::Tag tag) : tag_(tag), lexeme_() {}

Token::Token(Lexeme lexeme, Token::Tag tag) : lexeme_(std::move(lexeme)), tag_(tag) {}

const Lexeme &Token::getLexeme() const {
    return lexeme_;
}

Token::Tag Token::getTag() const {
    return tag_;
}

bool Token::operator==(const Token &rhs) const {
    return this->lexeme_ == rhs.lexeme_ && this->tag_ == rhs.tag_;
}

std::ostream &operator<<(std::ostream &output, const Token &token) {
    output << "Token(" << token.lexeme_ << ")";
    return output;
}
