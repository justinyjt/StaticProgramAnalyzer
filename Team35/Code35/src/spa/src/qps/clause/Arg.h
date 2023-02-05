#include "string"
#include "commons/types.h"

/* Base class for all args to clauses */
class Arg {
};

/*
"<expr>, _<expr>_, _"
*/ 
class ExpressionSpec : virtual public Arg {
public:
    ExpressionSpec(std::string str);

private:
    const std::string str;
};

/*
stmtRef : synonym | '_' | INTEGER
*/
class StmtRef : virtual public Arg {};

/*
entRef : synonym | '_' | '"' IDENT '"'
*/
class EntRef : virtual public Arg {};