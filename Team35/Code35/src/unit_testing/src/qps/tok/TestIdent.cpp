#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/pql/Ident.h"
#include "qps/pql/PQLToken.h"

TEST_CASE("Ident") {
    Ident *hi = new Ident("hi");
    Ident *bye = new Ident("bye");
    std::unique_ptr<PQLToken> hi_ = std::make_unique<Ident>("hi");

    requireTrue(*hi == *hi_);
    requireTrue(*hi != *bye);

    requireEqual(hi->tag, PQLToken::Tag::IDENT);
    requireEqual(hi->s, std::string("hi"));
    requireEqual(hi->str(), std::string("hi"));
    delete hi;
    delete bye;
}
