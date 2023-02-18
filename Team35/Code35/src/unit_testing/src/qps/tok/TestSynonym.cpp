#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/pql/Synonym.h"
#include "qps/pql/PQLToken.h"

TEST_CASE("Synonym") {
    std::unique_ptr<Synonym> x = std::make_unique<Synonym>(Synonym::DesignEntity::VARIABLE, "x");
    std::unique_ptr<Synonym> y = std::make_unique<Synonym>(Synonym::DesignEntity::VARIABLE, "y");
    std::unique_ptr<PQLToken> x_ = std::make_unique<Synonym>(Synonym::DesignEntity::VARIABLE, "x");

    requireTrue(*x == *x_);
    requireTrue(*x != *y);

    requireEqual(x->tag, PQLToken::Tag::SYNONYM);
    requireEqual(x->de, Synonym::DesignEntity::VARIABLE);
    requireEqual(x->ident, std::string("x"));
    requireEqual(x->str(), std::string("x"));
}
