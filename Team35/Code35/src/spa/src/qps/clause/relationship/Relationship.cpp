#include "Relationship.h"
#include "qps/pql/PQLToken.h"

Relationship::Relationship(PQLToken* first, PQLToken* second) : Clause(first, second) {}
