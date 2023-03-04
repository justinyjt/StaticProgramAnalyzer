#pragma once
#include <memory>
#include <utility>

#include "qps/clause/Clause.h"
#include "qps/pql/Synonym.h"

class SelectClause : public Clause {
 public:
    std::unique_ptr<Result> evaluate(PKBReader *db) override;
    bool operator==(const Clause &rhs) const override;
};
