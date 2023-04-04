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
    STMT_SET getStmtSet(PKBReader *db, Synonym synonym);

    ENT_SET getEntSet(PKBReader *db, Synonym synonym);

    STMT_ENT_SET getStmtEntSet(PKBReader *db, Synonym synonym);

    bool isEntAttrRef(Synonym &syn);

    std::vector<std::unique_ptr<Synonym>> selectedSynonyms_;
};
