#include "qps/result/Result.h"
#include "qps/pql/Tok.h"
#include "qps/clause/Clause.h"
#include "Relationship.h"

class UsesS : public Relationship {
public:
    /* <SYNONYM | STMT_NUM>, <SYNONYM | WILDCARD | IDENT_STR> */
    UsesS(Tok* first, Tok* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};

class UsesP : public Relationship {
public:
    /* <SYNONYM | IDENT_STR>, <SYNONYM | WILDCARD | IDENT_STR> */
    UsesP(Tok* first, Tok* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};

