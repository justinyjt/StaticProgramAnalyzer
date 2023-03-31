#pragma once
#include <memory>
#include <utility>

#include "qps/clause/Clause.h"
#include "qps/pql/Synonym.h"

class SelectClause : public Clause {
 public:
    enum class SelectType {
        Boolean,
        Synonym
    };
    const SelectType type_;
    explicit SelectClause(SelectClause::SelectType type);
};
