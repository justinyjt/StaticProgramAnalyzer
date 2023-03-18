#pragma once
#include <memory>
#include <utility>

#include "qps/clause/Clause.h"
#include "qps/pql/Synonym.h"

class SelectClause : public Clause {
 public:
    SelectClause() = default;
};
