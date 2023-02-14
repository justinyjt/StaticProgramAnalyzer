#include "qps/result/Result.h"
#include "qps/pql/Tok.h"
#include "qps/clause/Clause.h"
#include "Relationship.h"

class Parent : public Relationship {
public:
    /* <SYNONYM | STMT_NUM>, <SYNONYM | WILDCARD | IDENT_STR> */
    Parent(Tok* first, Tok* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};

class ParentT : public Relationship {
public:
    /* <SYNONYM | IDENT_STR>, <SYNONYM | WILDCARD | IDENT_STR> */
    ParentT(Tok* first, Tok* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
