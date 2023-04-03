#pragma once

#include <cstdint>
#include <string>
#include <unordered_set>

#include "Clause.h"

class OptimisableClause : public Clause {
 public:
    enum class Complexity {
        Low, Medium, High
    };
    OptimisableClause();
    virtual ~OptimisableClause() = default;
    Complexity getComplexity() const;

 protected:
    void setComplexity(Complexity complexity);

 private:
    Complexity complexity_;
};
