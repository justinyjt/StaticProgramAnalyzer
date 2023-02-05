
/*
Base class for all PQL tokens
*/
class Tok {
  public:
    enum Tag { IDENT, STMT_NUM, SYNONYM, WILDCARD, EXPR };

    Tok(Tag tag) : tag(tag) {};
    const Tag tag;
};