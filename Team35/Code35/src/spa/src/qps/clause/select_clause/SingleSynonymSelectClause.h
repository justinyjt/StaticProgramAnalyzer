#pragma once
#include <memory>
#include <string>
#include <utility>

#include "SelectClause.h"
#include "qps/pql/Synonym.h"

class SingleSynonymSelectClause : public SelectClause {
 public:
    const std::unique_ptr<Synonym> syn;
    std::string selectedIdent;
    explicit SingleSynonymSelectClause(std::unique_ptr<Synonym> syn);
    std::unique_ptr<Result> evaluate(PKBReader *db);
    bool operator==(const Clause &rhs) const override;
};
