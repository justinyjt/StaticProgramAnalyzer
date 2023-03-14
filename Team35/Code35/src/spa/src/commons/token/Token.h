#pragma once

#include <cstdint>
#include <iostream>
#include <string>

#include "commons/types.h"

typedef std::string Lexeme;

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
        Else,
        Then,
        Statement,
        Variable,
        Constant,
        Assign,
        Such,
        That,
        Select,
        Bool,
        Follows,
        Modifies,
        Uses,
        Calls,
        Parent,
        Pattern,

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
        LBrace,
        RBrace,
        SemiColon,
        Underscore,
        DoubleQuotes,
        Comma,
        Star,

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

    Lexeme getLexeme() const;
    Tag getTag() const;
    LineNumber getLineNumber() const;

    bool operator==(Token const &rhs) const;
    friend std::ostream &operator<<(std::ostream &output, const Token &token);

 private:
    Lexeme lexeme_;
    Tag tag_;
    LineNumber line_number_ = null_line_number_;
};
