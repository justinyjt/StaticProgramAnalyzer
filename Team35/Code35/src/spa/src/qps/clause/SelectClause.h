#pragma once
#include <memory>
#include <utility>

#include "qps/clause/Clause.h"
#include "qps/pql/Synonym.h"

class SelectClause : public Clause {
 public:
    Synonym const syn;
    explicit SelectClause(Synonym &syn);
    std::unique_ptr<Result> evaluate(PKBReader *db) override;
    bool operator==(const Clause &rhs) const override;
};
