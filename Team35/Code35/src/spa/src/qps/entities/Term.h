#pragma once

class Term {
 public:
    enum class Tag {
        Synonym, StatementNumber, Variable, Wildcard, Expression
    };

    explicit Term(Term::Tag tag);

    Tag getTag();
 private:
    Tag tag;
};

