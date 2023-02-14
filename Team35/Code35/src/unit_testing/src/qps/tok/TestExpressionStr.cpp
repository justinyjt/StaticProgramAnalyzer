#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/pql/ExpressionStr.h"
#include "qps/pql/PQLToken.h"

TEST_CASE("ExpressionStr") {
    ExpressionStr* st1 = new ExpressionStr("_x+1_");
    ExpressionStr* st2 = new ExpressionStr("_y+1_");
    PQLToken* st1_ = new ExpressionStr("_x+1_");

    requireTrue(*st1 == *st1_);
    requireTrue(*st1 != *st2);

    requireEqual(st1->tag, PQLToken::Tag::EXPR);
    requireEqual(st1->s, std::string("_x+1_"));
    requireEqual(st1->str(), std::string("_x+1_"));
}
