#pragma once

#include <memory>

#include "qps/clause/OptimisableClause.h"

class MockOptimisableClause : public OptimisableClause {
 public:
    MockOptimisableClause(OptimisableClause::Complexity complexity, const HEADER_SET &headers);
    ~MockOptimisableClause() override = default;
    std::unique_ptr<Result> evaluate(PKBReader *db) override;
    bool operator==(const Clause &rhs) const override;
};
