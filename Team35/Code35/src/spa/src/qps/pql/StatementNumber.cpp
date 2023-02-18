#include "StatementNumber.h"

StatementNumber::StatementNumber(int n) : PQLToken::PQLToken(Tag::STMT_NUM), n(n) {}

std::string StatementNumber::str() const {
    return std::to_string(n);
}

bool StatementNumber::operator==(const PQLToken& rhs) const {
    const auto* p_rhs = dynamic_cast<const StatementNumber*>(&rhs);
    return p_rhs != nullptr && PQLToken::equal(rhs) && n == p_rhs->n;
}
