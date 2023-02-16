#include "Token.h"

#include <utility>

Token::Token(Token::Tag tag) : tag_(tag), lexeme_() {}

Token::Token(Lexeme lexeme, Token::Tag tag) : lexeme_(std::move(lexeme)), tag_(tag) {}

Token::Token(Tag tag, LineNumber line_number) : tag_(tag), line_number_(line_number) {}

Token::Token(Lexeme lexeme, Tag tag, LineNumber line_number)
    : lexeme_(std::move(lexeme)), tag_(tag), line_number_(line_number) {}

Token::Token(const Token &token, LineNumber line_number)
    : lexeme_(token.lexeme_), tag_(token.tag_), line_number_(line_number) {}

Lexeme Token::getLexeme() const {
    return lexeme_;
}

Token::Tag Token::getTag() const {
    return tag_;
}

LineNumber Token::getLineNumber() const {
    return line_number_;
}

bool Token::operator==(const Token &rhs) const {
    return this->lexeme_ == rhs.lexeme_ && this->tag_ == rhs.tag_ && this->line_number_ == rhs.line_number_;
}

std::ostream &operator<<(std::ostream &output, const Token &token) {
    output << "Token(" << token.lexeme_ << ", " << token.line_number_ << ")";
    return output;
}
