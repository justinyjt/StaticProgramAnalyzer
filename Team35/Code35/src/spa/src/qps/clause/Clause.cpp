#include "Clause.h"
#include "qps/pql/Tok.h"
#include "qps/query_exceptions/SemanticException.h"

Clause::Clause(Tok* first, Tok* second) : first(first), second(second) {
}

bool Clause::equal(const Clause &rhs) const {
    return *first == *(rhs.first) && *second == *(rhs.second);
}

bool Clause::operator!=(const Clause &rhs) const {
    return !(*this == rhs);
}
