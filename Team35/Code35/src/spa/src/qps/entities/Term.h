class Term {
public:
    enum class Tag {
        Synonym, StatementNumber, Variable, Wildcard, Expression
    };

    Term(Term::Tag tag);

    Tag getTag();
private:
    Tag tag;
};

