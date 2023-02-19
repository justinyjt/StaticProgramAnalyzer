#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/pql/Expression.h"
#include "qps/pql/PQLToken.h"

TEST_CASE("Expression") {
    Expression *st1 = new Expression("x+1", false);
    Expression *st2 = new Expression("y+1", false);
    std::unique_ptr<PQLToken> st1_ = std::make_unique<Expression>("x+1", false);

    requireTrue(*st1 == *st1_);
    requireTrue(*st1 != *st2);

    requireEqual(st1->tag, PQLToken::Tag::EXPR);
    requireEqual(st1->s, std::string("x+1"));
    requireEqual(st1->str(), std::string("x+1"));
    delete st1;
    delete st2;
}
