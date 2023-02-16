#include "Clause.h"

bool Clause::operator!=(const Clause &rhs) const {
    return !(*this == rhs);
}
