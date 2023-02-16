#pragma once
#include "qps/clause/Clause.h"

class SelectClause : public Clause {
 public:
    Synonym const syn;
    explicit SelectClause(Synonym syn);
    Result* evaluate(PKBReader* db);
    bool operator==(const Clause& rhs) const;
};
