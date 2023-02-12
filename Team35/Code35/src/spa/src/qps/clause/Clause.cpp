#include "Clause.h"
#include "qps/pql/Tok.h"

Clause::Clause(Tok* first, Tok* second) : first(first), second(second) {}

bool Clause::equal(const Clause &rhs) const {
    return *first == *(rhs.first) && *second == *(rhs.second);
}
