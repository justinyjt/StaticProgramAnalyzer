#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/pql/Wildcard.h"
#include "qps/pql/PQLToken.h"

TEST_CASE("Wildcard") {
    Wildcard* w = new Wildcard();
    PQLToken* w_ = new Wildcard();

    requireTrue(*w == *w_);
    requireEqual(w->str(), std::string("_"));
}
