#include "qps/clauses/Clause.h"
#include "commons/types.h"
#include "qps/entities/Term.h"

class Relationship : public Clause {
public:
    Relationship(Term first, Term second);
private:
    Term first;
    Term second;
};

