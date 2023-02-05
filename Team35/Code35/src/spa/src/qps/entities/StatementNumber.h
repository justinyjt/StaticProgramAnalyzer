#pragma once

#include "Term.h"

class StatementNumber : public Term {
 public:
    explicit StatementNumber(int statementNumber);

    int getStatementNumber() const;
 private:
    int statementNumber;
};
