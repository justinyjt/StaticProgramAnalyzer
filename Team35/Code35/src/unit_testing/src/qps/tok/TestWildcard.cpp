#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/pql/Wildcard.h"
#include "qps/pql/Tok.h"

TEST_CASE("Wildcard") {
    Wildcard* w = new Wildcard();
    Tok* w_ = new Wildcard();

    requireTrue(*w == *w_);
    requireEqual(w->str(), std::string("_"));
}
