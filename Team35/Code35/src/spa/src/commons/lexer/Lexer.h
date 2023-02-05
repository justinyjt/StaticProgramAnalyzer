#pragma once

#include "ILexer.h"

#include <list>
#include <string>
#include <unordered_map>

#include "commons/token/Character.h"
#include "commons/token/Keyword.h"

typedef std::string Source;
typedef std::unordered_map<std::string, Keyword> KeywordMap;
typedef std::unordered_map<char, Character> CharacterMap;
typedef std::list<Keyword> KeywordList;
typedef std::list<Character> CharacterList;

class Lexer : public ILexer {
 public:
    explicit Lexer(Source source,
                   const KeywordList &keyword_list,
                   const CharacterList &character_list,
                   bool include_operator = true,
                   bool include_string = true);
    Token scan();
    ~Lexer() = default;

 protected:
    LineNumber getCurrentLineNumber() const;
    char readChar();
    bool readChar(char c);
    char peekChar();
    void unreadChar();
    void skipChar();
    bool isEof() const;
    void reserve(const Keyword &keyword);
    void reserve(const Character &character);

 private:
    Source source_;
    uint32_t current_position_ = 0;
    uint32_t current_line_ = 1;
    KeywordMap keyword_map_;
    CharacterMap character_map_;
    bool include_operator_;
    bool include_string_;

    Token scanNextName();
    Token scanNextInteger();
    Token scanNextString();
    Token scanNextCharacter();

    bool isNameStart(char c) const;
    bool isNamePart(char c) const;
    bool isDigit(char c) const;
    bool isStringStartEnd(char c) const;
    bool isNewLine(char c) const;
    bool isControlOrSpace(char c) const;
};
