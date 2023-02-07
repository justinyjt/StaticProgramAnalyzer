#include "StatementNumber.h"

StatementNumber::StatementNumber(int n) : Tok::Tok(STMT_NUM), n(n) {}

std::string StatementNumber::str() const {
    return std::to_string(n);
}
