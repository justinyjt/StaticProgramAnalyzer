#include "Modify.h"

Modify::Modify(StmtRef& first, EntRef& second) : Relationship(dynamic_cast<Arg&>(first), 
                                                        dynamic_cast<Arg&>(second)) {}

// TODO SPRINT 1
// select v such that modifies(1, v)

Result Modify::evaluate() {
  // if first is stmtNum and second is synonym
  std::unordered_map<std::string> = db.getAllRelationships(StmtNameRelationship::Modifies, 1);
  // for (e
}