#include "Modify.h"
#include "qps/pql/StatementNumber.h"

Modify::Modify(Tok& first, Tok& second) : Relationship(first, second) {}

// TODO SPRINT 1
// select v such that modifies(1, v)

Result Modify::evaluate(PKBReader* db) {
  if (first.tag == Tok::STMT_NUM && second.tag == Tok::SYNONYM) {
    ENT_SET set = db->getRelationship(StmtNameRelationship::Modifies, 
                        static_cast<StatementNumber&>(first).stmtNum);
    StrResult result(set);
    return (result);
  }
    Result r;
    return r;
}

bool Modify::operator==(const Modify &rhs) const {
    std::cout << "hi";
    return this->first.tag == rhs.first.tag && this->second.tag == rhs.second.tag;
}
