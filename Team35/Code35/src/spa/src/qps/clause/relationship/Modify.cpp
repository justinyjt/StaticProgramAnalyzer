#include "Modify.h"
#include "qps/pql/StatementNumber.h"
#include <iostream>

Modify::Modify(Tok first, Tok second) : Relationship(first, second) {}

// TODO SPRINT 1
// select v such that modifies(1, v)

Result* Modify::evaluate(PKBReader* db) {
  if (first.tag == Tok::STMT_NUM && second.tag == Tok::SYNONYM) {
    ENT_SET set = db->getRelationship(StmtNameRelationship::Modifies, 
                        static_cast<StatementNumber&>(first).stmtNum);
    StrResult* result = new StrResult(set);
    return (result);
  }
  
  throw std::runtime_error("");
}

bool Modify::operator==(const Modify &rhs) const {
    return this->first.tag == rhs.first.tag && this->second.tag == rhs.second.tag;
}
