#include "Modify.h"
#include "qps/pql/StatementNumber.h"

Modify::Modify(Tok first, Tok second) : Relationship(first, second) {}

// select v such that modifies(1, v)

Result* Modify::evaluate(PKBReader* db) {
  if (first.tag == Tok::STMT_NUM && second.tag == Tok::SYNONYM) {
    ENT_SET set = db->getRelationship(StmtNameRelationship::Modifies,
                                      std::stoi(first.getValue()));
    // temporary workaround
    StrResult* result = new StrResult(set);
    return dynamic_cast<Result*>(result);
  }

  throw std::runtime_error("");
}

bool Modify::operator==(const Modify &rhs) const {
    return this->first.tag == rhs.first.tag && this->second.tag == rhs.second.tag;
}
