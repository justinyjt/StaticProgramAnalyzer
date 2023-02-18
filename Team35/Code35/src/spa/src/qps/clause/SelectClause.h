#pragma once
#include <memory>
#include "qps/clause/Clause.h"

class SelectClause : public Clause {
 public:
    Synonym const syn;
    explicit SelectClause(Synonym& syn);
    std::unique_ptr<Result> evaluate(PKBReader* db) override;
    bool operator==(const Clause& rhs) const override;
};
