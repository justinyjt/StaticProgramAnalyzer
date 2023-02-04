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
    explicit Lexer(Source source, KeywordList keyword_list, CharacterList character_list);
    Token Scan();
    ~Lexer() = default;

 protected:
    char ReadChar();
    bool ReadChar(char c);
    char PeekChar();
    char PeekNextChar();
    void SkipChar();

 private:
    Source source_;
    uint32_t current_position_ = 0;
    uint32_t current_line_ = 1;
    KeywordMap keyword_map_;
    CharacterMap character_map_;

    Token ScanNextName(char first_char);
    Token ScanNextInteger(char first_char);
    Token ScanNextString();
    Token ScanNextCharacter(char first_char);

    bool IsNameStart(char c);
    bool IsNamePart(char c);
    bool IsDigit(char c);
    bool IsDoubleQuotes(char c);
    bool IsNewLine(char c);
    bool IsControlOrSpace(char c);
    bool IsEOF();
};
