#pragma once
#include <memory>
#include <vector>
#include <utility>

#include "SelectClause.h"
#include "qps/pql/Synonym.h"

class MultipleSynonymSelectClause : public SelectClause {
 public:
    explicit MultipleSynonymSelectClause(std::vector<std::unique_ptr<Synonym>> selectedSynonyms);
    std::unique_ptr<Result> evaluate(PKBReader *db) override;
    bool operator==(const Clause &rhs) const override;

 private:
    std::vector<std::unique_ptr<Synonym>> selectedSynonyms_;
};
