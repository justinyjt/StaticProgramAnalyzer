#include "Clause.h"
#include "qps/pql/Tok.h"

Clause::Clause(Tok* first, Tok* second) : first(first), second(second) {}

bool Clause::operator==(const Clause &rhs) const {
    return true;
}
