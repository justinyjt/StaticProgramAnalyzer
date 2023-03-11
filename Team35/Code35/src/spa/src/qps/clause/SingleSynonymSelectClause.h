#pragma once
#include <memory>
#include <utility>

#include "qps/clause/SelectClause.h"
#include "qps/pql/Synonym.h"

class SingleSynonymSelectClause : public SelectClause {
 public:
    const Synonym syn;
    explicit SingleSynonymSelectClause(Synonym &syn);
    std::unique_ptr<Result> evaluate(PKBReader *db);
    bool operator==(const Clause &rhs) const;
};
