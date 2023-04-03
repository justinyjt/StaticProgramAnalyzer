#pragma once

#include <iostream>
#include <string>

#include "commons/types.h"

typedef std::string Lexeme;

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
        Next,
        Affects,
        Parent,
        Pattern,
        With,
        And,

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
        Comma,
        Star,
        Dot,
        Hex,

        // types
        Ascii,
        Integer,
        String,

        // misc
        EndOfFile
    };

    explicit Token(Tag tag);
    Token(Lexeme lexeme, Tag tag);

    const Lexeme &getLexeme() const;
    Tag getTag() const;

    bool operator==(Token const &rhs) const;
    friend std::ostream &operator<<(std::ostream &output, const Token &token);

 private:
    Lexeme lexeme_;
    Tag tag_;
};
