#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/pql/Synonym.h"
#include "qps/pql/Tok.h"

TEST_CASE("Synonym") {
    Synonym* x = new Synonym(Synonym::DesignEntity::VARIABLE, "x");
    Synonym* y = new Synonym(Synonym::DesignEntity::VARIABLE, "y");
    Tok* x_ = new Synonym(Synonym::DesignEntity::VARIABLE, "x");

    requireTrue(*x == *x_);
    requireTrue(*x != *y);

    requireEqual(x->tag, Tok::Tag::SYNONYM);
    requireEqual(x->de, Synonym::DesignEntity::VARIABLE);
    requireEqual(x->ident, std::string("x"));
    requireEqual(x->str(), std::string("x"));
}
