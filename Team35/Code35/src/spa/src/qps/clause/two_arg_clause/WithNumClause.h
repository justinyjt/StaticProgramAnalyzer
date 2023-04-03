#pragma once

#include <memory>
#include <string>
#include <utility>

#include "TwoArgClause.h"

class WithNumClause : public TwoArgClause {
 public:
    /* <stmt SYNONYM | name SYNONYM | NUM | ENT>, <stmt SYNONYM | name SYNONYM | NUM | ENT> */
    WithNumClause(std::unique_ptr<PQLToken>, std::unique_ptr<PQLToken>);

    std::unique_ptr<Result> evaluate(PKBReader *) override;

    void validateArgs() override;

 protected:
    static STMT_SET getStmtNumsFromSyn(Synonym syn, PKBReader *db);

    static std::unique_ptr<Result> handleSynNumCase(PKBReader *db, Synonym syn, std::string num);

    static std::unique_ptr<Result> handleSynSynCase(PKBReader *db, Synonym syn1, Synonym syn2);

    static std::unique_ptr<Result> handleTwoConstCase(PKBReader *db, std::string syn1, std::string syn2);

    static std::unique_ptr<Result> handleOneConstCaseSyn(PKBReader *db, std::string constSyn, Synonym nonConstSyn);

    static std::unique_ptr<Result> handleNoConstCaseSyn(PKBReader *db, Synonym syn1, Synonym syn2);

    static std::unique_ptr<Result> handleOneConstCaseNum(PKBReader *db, std::string constSyn, std::string num);

    static std::unique_ptr<Result> handleNoConstCaseNum(PKBReader *db, Synonym syn, STMT_NUM num);

    static std::unique_ptr<Result> handleSameSynCase(PKBReader *db, Synonym syn);


    bool operator==(const Clause &rhs) const override;
};
