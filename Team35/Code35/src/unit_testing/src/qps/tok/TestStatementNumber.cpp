#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/pql/StatementNumber.h"
#include "qps/pql/Tok.h"

TEST_CASE("StatementNumber") {
    StatementNumber* st1 = new StatementNumber(1);
    StatementNumber* st2 = new StatementNumber(2);
    Tok* st1_ = new StatementNumber(1);

    requireTrue(*st1 == *st1);
    requireTrue(*st1 != *st2);

    requireEqual(st1->tag, Tok::Tag::STMT_NUM);
    requireEqual(st1->n, 1);
    requireEqual(st1->str(), std::string("1"));
}
