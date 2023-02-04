#pragma once

#include <cstdint>
#include <string>

typedef std::string Lexeme;
typedef std::uint32_t LineNumber;

class Token {
 public:
    static const LineNumber null_line_number_ = 0;
    enum class Tag {
      // keywords
      Keyword,
      Name,
      Read,
      Print,
      Call,
      If,
      While,
      Procedure,

      // logical operators
      LogicalAnd,
      LogicalOr,
      LogicalNot,

      // relational operators
      Assignment,
      Equivalence,
      NotEqual,
      LessThan,
      LessThanEqualTo,
      GreaterThan,
      GreaterThanEqualTo,

      // arithmetic operators
      Plus,
      Minus,
      Divide,
      Multiply,
      Modulo,
      Equal,

      // other symbols
      LParen,
      RParen,
      LBraces,
      RBraces,
      SemiColon,
      Underscore,
      DoubleQuotes,
      Comma,

      // types
      Ascii,
      Integer,
      String,

      // misc
      NewLine,
      EndOfFile
    };

    explicit Token(Tag tag);
    Token(Lexeme lexeme, Tag tag);
    Token(Tag tag, LineNumber line_number);
    Token(Lexeme lexeme, Tag tag, LineNumber line_number);
    Token(const Token &token, LineNumber line_number);

    Lexeme GetLexeme() const;
    Tag GetTag() const;
    LineNumber GetLineNumber() const;

    bool operator==(Token const &rhs) const;

 private:
    Lexeme lexeme_ = "";
    Tag tag_;
    LineNumber line_number_ = null_line_number_;
};
