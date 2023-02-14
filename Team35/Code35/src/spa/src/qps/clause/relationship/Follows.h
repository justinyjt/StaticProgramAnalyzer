#include "Relationship.h"
#include "qps/result/Result.h"

class Follows : public Relationship {
public:
    /* <SYNONYM | STMT_NUM>, <SYNONYM | WILDCARD | IDENT_STR> */
    Follows(Tok* first, Tok* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};

class FollowsT : public Relationship {
public:
    /* <SYNONYM | IDENT_STR>, <SYNONYM | WILDCARD | IDENT_STR> */
    FollowsT(Tok* first, Tok* second);
    Result* evaluate(PKBReader*);
    bool operator==(const Clause& rhs) const;
};
