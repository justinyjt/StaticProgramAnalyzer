#pragma once

#include <memory>
#include <utility>

#include "TwoArgClause.h"

class WithEntClause : public TwoArgClause {
 public:
    /* <stmt SYNONYM | name SYNONYM | NUM | ENT>, <stmt SYNONYM | name SYNONYM | NUM | ENT> */
    WithEntClause(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>);

    std::unique_ptr<Result> evaluate(PKBReader *) override;

    void validateArgs() override;

 protected:
    ENT_SET getEntValuesFromSyn(Synonym syn, PKBReader *db);

    bool operator==(const Clause &rhs) const override;
};
