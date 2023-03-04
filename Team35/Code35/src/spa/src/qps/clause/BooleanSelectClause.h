#pragma once
#include <memory>
#include <utility>

#include "qps/clause/Clause.h"
#include "qps/pql/Synonym.h"
#include "SelectClause.h"

class BooleanSelectClause : public SelectClause {
 public:
    BooleanSelectClause();
    std::unique_ptr<Result> evaluate(PKBReader *db) override;
    bool operator==(const Clause &rhs) const override;
};
