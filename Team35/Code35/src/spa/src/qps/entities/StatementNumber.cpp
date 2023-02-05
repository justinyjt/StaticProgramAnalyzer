#include "StatementNumber.h"

StatementNumber::StatementNumber(int statementNumber) : Term::Term(Term::Tag::StatementNumber),
        statementNumber(statementNumber) {}

int StatementNumber::getStatementNumber() const {
    return statementNumber;
}
