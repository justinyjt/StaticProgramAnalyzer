#include "Lexer.h"

#include <cassert>
#include <cctype>
#include <utility>

#include "commons/lexer/exception/LexerException.h"
#include "commons/token/Integer.h"
#include "commons/util/ConversionUtil.h"

Lexer::Lexer(Source source,
             const KeywordList &keyword_list,
             const CharacterList &character_list,
             bool include_operator,
             bool include_string)
    : source_(std::move(source)),
      keyword_map_(),
      character_map_(),
      include_operator_(include_operator),
      include_string_(include_string) {
    for (const auto &keyword : keyword_list) {
        reserve(keyword);
    }
    for (const auto &character : character_list) {
        reserve(character);
    }
}

Token Lexer::scan() {
    if (isEof()) {
        return Token(Token::Tag::EndOfFile, getCurrentLineNumber());
    }
    while (!isEof() && isControlOrSpace(peekChar())) {
        skipChar();
    }

    if (isEof()) {
        return Token(Token::Tag::EndOfFile, getCurrentLineNumber());
    }

    if (include_operator_) {
        char character = readChar();
        switch (character) {
            case '&':
                if (readChar('&')) {
                    return Token("&&", Token::Tag::LogicalAnd, getCurrentLineNumber());
                }
                break;
            case '|':
                if (readChar('|')) {
                    return Token("||", Token::Tag::LogicalOr, getCurrentLineNumber());
                }
                break;
            case '=':
                if (readChar('=')) {
                    return Token("==", Token::Tag::Equivalence, getCurrentLineNumber());
                }
                break;
            case '!':
                if (readChar('=')) {
                    return Token("!=", Token::Tag::NotEqual, getCurrentLineNumber());
                }
                return Character(character, Token::Tag::LogicalNot, getCurrentLineNumber());
            case '<':
                if (readChar('=')) {
                    return Token("<=", Token::Tag::LessThanEqualTo, getCurrentLineNumber());
                }
                return Token("<", Token::Tag::LessThan, getCurrentLineNumber());
            case '>':
                if (readChar('=')) {
                    return Token(">=", Token::Tag::GreaterThanEqualTo, getCurrentLineNumber());
                }
                return Token(">", Token::Tag::GreaterThan, getCurrentLineNumber());
            default:break;
        }
        unreadChar();
    }

    char character = peekChar();

    if (isDigit(character)) {
        return scanNextInteger();
    }

    if (isNameStart(character)) {
        return scanNextName();
    }

    if (include_string_ && isStringStartEnd(character)) {
        return scanNextString();
    }

    return scanNextCharacter();
}

LineNumber Lexer::getCurrentLineNumber() const {
    return current_line_;
}

char Lexer::readChar() {
    char c = peekChar();
    skipChar();
    return c;
}

bool Lexer::readChar(char c) {
    if (peekChar() != c) {
        return false;
    }
    skipChar();
    return true;
}

char Lexer::peekChar() {
    return source_[current_position_];
}

void Lexer::unreadChar() {
    if (current_position_ == 0) {
        return;
    }
    current_position_--;
    if (isNewLine(peekChar())) {
        current_line_--;
    }
}

void Lexer::skipChar() {
    if (isNewLine(peekChar())) {
        current_line_++;
    }
    current_position_++;
}

void Lexer::reserve(const Keyword &keyword) {
    keyword_map_.insert(std::make_pair(keyword.getLexeme(), keyword));
}

void Lexer::reserve(const Character &character) {
    character_map_.insert(std::make_pair(character.getCharacter(), character));
}

bool Lexer::isEof() const {
    return current_position_ >= source_.length();
}

Token Lexer::scanNextName() {
    assert(isNameStart(peekChar()));
    std::string lexeme;
    lexeme += readChar();
    while (!isEof() && isNamePart(peekChar())) {
        lexeme += readChar();
    }
    if (keyword_map_.find(lexeme) != keyword_map_.end()) {
        return Keyword(keyword_map_.at(lexeme), getCurrentLineNumber());
    }
    return Token(lexeme, Token::Tag::Name, getCurrentLineNumber());
}

Token Lexer::scanNextInteger() {
    assert(isDigit(peekChar()));
    int number = 0;
    while (!isEof() && isDigit(peekChar())) {
        number *= 10;
        number += ConvertCharToInt(readChar());
    }
    return Integer(number, getCurrentLineNumber());
}

Token Lexer::scanNextCharacter() {
    char character = readChar();
    if (character_map_.find(character) == character_map_.end()) {
        throw LexerException("Invalid character", getCurrentLineNumber());
    }
    return Character(character_map_.at(character), getCurrentLineNumber());
}

Token Lexer::scanNextString() {
    assert(isStringStartEnd(peekChar()));
    skipChar();
    std::string lexeme;
    while (!isEof() && !isStringStartEnd(peekChar())) {
        lexeme += readChar();
    }
    if (isEof()) {
        throw LexerException("Unterminated string", getCurrentLineNumber());
    }
    skipChar();
    return Token(lexeme, Token::Tag::String, getCurrentLineNumber());
}

bool Lexer::isNameStart(char c) const {
    return isalpha(c);
}

bool Lexer::isNamePart(char c) const {
    return isalnum(c);
}

bool Lexer::isDigit(char c) const {
    return isdigit(c);
}

bool Lexer::isStringStartEnd(char c) const {
    return c == '"';
}

bool Lexer::isNewLine(char c) const {
    return c == '\n';
}

bool Lexer::isControlOrSpace(char c) const {
    return iscntrl(c) || isspace(c);
}
