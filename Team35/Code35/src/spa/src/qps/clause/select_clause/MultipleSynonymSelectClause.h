#pragma once
#include <memory>
#include <vector>
#include <utility>

#include "SelectClause.h"
#include "qps/pql/Synonym.h"
#include "SingleSynonymSelectClause.h"

class MultipleSynonymSelectClause : public SelectClause {
 public:
    std::vector<std::unique_ptr<SingleSynonymSelectClause>> selectClauses;
    explicit MultipleSynonymSelectClause(std::vector<std::unique_ptr<SingleSynonymSelectClause>>);
    std::unique_ptr<Result> evaluate(PKBReader *db);
    bool operator==(const Clause &rhs) const;
};
