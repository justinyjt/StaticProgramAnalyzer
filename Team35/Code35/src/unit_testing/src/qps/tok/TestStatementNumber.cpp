#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/pql/StatementNumber.h"
#include "qps/pql/PQLToken.h"

TEST_CASE("StatementNumber") {
    std::unique_ptr<StatementNumber> st1 = std::make_unique<StatementNumber>(1);
    std::unique_ptr<StatementNumber> st2 = std::make_unique<StatementNumber>(2);
    std::unique_ptr<PQLToken> st1_ = std::make_unique<StatementNumber>(1);

    requireTrue(*st1 == *st1);
    requireTrue(*st1 != *st2);

    requireEqual(st1->tag, PQLToken::Tag::STMT_NUM);
    requireEqual(st1->n, 1);
    requireEqual(st1->str(), std::string("1"));
}
