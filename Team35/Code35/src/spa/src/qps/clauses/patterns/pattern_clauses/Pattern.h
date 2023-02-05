#pragma once

#include <string>
#include "qps/clauses/Clause.h"
#include "qps/entities/Term.h"

class Pattern : public Clause {public:
    Pattern(Term first, Term second);
 private:
    Term first;
    Term second;
};
