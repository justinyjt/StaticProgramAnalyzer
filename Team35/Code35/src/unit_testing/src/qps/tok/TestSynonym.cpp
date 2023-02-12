#include "catch.hpp"
#include "../../TestHelper.h"
#include "qps/pql/Synonym.h"
#include "qps/pql/Tok.h"

TEST_CASE("Synonym") {
    Synonym* st1 = new Synonym(Synonym::DesignEntity::STMT, "s");
    Synonym* st2 = new Synonym(Synonym::DesignEntity::STMT, "t");
    Tok* st1_ = new Synonym(Synonym::DesignEntity::STMT, "s");

    requireTrue(*st1 == *st1_);
    requireTrue(*st1 != *st2);

    requireEqual(st1->tag, Tok::Tag::STMT_NUM);
    requireEqual(st1->de, Synonym::DesignEntity::STMT);
    requireEqual(st1->ident, std::string("s"));
    requireEqual(st1->str(), std::string("s"));
}
