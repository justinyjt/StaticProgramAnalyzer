#include "PQLNumber.h"

PQLNumber::PQLNumber(std::string n) : PQLToken::PQLToken(Tag::STMT_NUM), n(n) {}

std::string PQLNumber::str() const {
    return n;
}

bool PQLNumber::operator==(const PQLToken &rhs) const {
    const PQLNumber *p_rhs = dynamic_cast<const PQLNumber *>(&rhs);
    return p_rhs != nullptr && n == p_rhs->n;
}
