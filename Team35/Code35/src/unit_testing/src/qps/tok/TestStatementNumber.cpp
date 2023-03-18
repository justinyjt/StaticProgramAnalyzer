#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/pql/PQLNumber.h"
#include "qps/pql/PQLToken.h"

TEST_CASE("PQLNumber") {
    std::unique_ptr<PQLNumber> st1 = std::make_unique<PQLNumber>("1");
    std::unique_ptr<PQLNumber> st2 = std::make_unique<PQLNumber>("2");
    std::unique_ptr<PQLNumber> st1_ = std::make_unique<PQLNumber>("1");

    requireTrue(*st1 == *st1);
    requireTrue(*st1 != *st2);

    requireEqual(st1->tag, PQLToken::Tag::STMT_NUM);
    requireEqual(st1->n, std::string("1"));
    requireEqual(st1->str(), std::string("1"));
}
