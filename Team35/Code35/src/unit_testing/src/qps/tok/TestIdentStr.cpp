#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/pql/IdentStr.h"
#include "qps/pql/Tok.h"

TEST_CASE("IdentStr") {
    IdentStr* hi = new IdentStr("hi");
    IdentStr* bye = new IdentStr("bye");
    Tok* hi_ = new IdentStr("hi");

    requireTrue(*hi == *hi_);
    requireTrue(*hi != *bye);

    requireEqual(hi->tag, Tok::Tag::IDENT);
    requireEqual(hi->s, std::string("hi"));
    requireEqual(hi->str(), std::string("hi"));
}
