#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/pql/Ident.h"
#include "qps/pql/PQLToken.h"

TEST_CASE("Ident") {
    Ident* hi = new Ident("hi");
    Ident* bye = new Ident("bye");
    PQLToken* hi_ = new Ident("hi");

    requireTrue(*hi == *hi_);
    requireTrue(*hi != *bye);

    requireEqual(hi->tag, PQLToken::Tag::IDENT);
    requireEqual(hi->s, std::string("hi"));
    requireEqual(hi->str(), std::string("hi"));
}
