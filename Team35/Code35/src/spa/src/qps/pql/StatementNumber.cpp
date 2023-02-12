#include "StatementNumber.h"

StatementNumber::StatementNumber(int n) : Tok::Tok(Tag::STMT_NUM), n(n) {}

std::string StatementNumber::str() const {
    return std::to_string(n);
}

bool StatementNumber::operator==(const Tok& rhs) const {
    const StatementNumber* p_rhs = dynamic_cast<const StatementNumber*>(&rhs);
    return p_rhs != NULL && Tok::equal(rhs) && n == p_rhs->n;
}
