#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/pql/Wildcard.h"
#include "qps/pql/PQLToken.h"

TEST_CASE("Wildcard") {
    std::unique_ptr<Wildcard> w = std::make_unique<Wildcard>();
    std::unique_ptr<PQLToken> w_ = std::make_unique<Wildcard>();

    requireTrue(*w == *w_);
    requireEqual(w->str(), std::string("_"));
}
