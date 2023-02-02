#pragma once

#include <cstdint>
#include <string>

typedef std::string Lexeme;
typedef std::uint32_t LineNumber;

class Token {
 public:
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
      EndOfFile
    };
    Token(Lexeme lexeme, Tag tag, LineNumber line_number);
    Token(Tag tag, LineNumber line_number);
    Lexeme GetLexeme();
    Tag GetTag();
    LineNumber GetLineNumber();

 private:
    Lexeme lexeme_ = "";
    Tag tag_;
    LineNumber line_number_;
};
