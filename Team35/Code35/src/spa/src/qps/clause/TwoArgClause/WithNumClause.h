#pragma once
#include <memory>
#include <utility>

#include "TwoArgClause.h"

class WithNumClause : public TwoArgClause {
public:
    /* <stmt SYNONYM | name SYNONYM | NUM | ENT>, <stmt SYNONYM | name SYNONYM | NUM | ENT> */
    WithNumClause(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>);
    std::unique_ptr<Result> evaluate(PKBReader *) override;
    void validateArgs() override;
protected:
    STMT_SET getNumValuesFromSyn(Synonym syn, PKBReader* db);
    bool operator==(const Clause &rhs) const override;
};
