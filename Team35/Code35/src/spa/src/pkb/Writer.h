#pragma once

#include "PKB.h"

#include <string>
#include <unordered_map>
#include <utility>
class Writer {
 public:
    explicit Writer(PKB &);

    bool addEntities(Entity entity, ENT_SET entitySet);
    bool addStmtEntityRelationships(StmtNameRelationship tableType, STMT_ENT_SET set);
    bool addEntityEntityRelationships(StmtNameRelationship tableType, ENT_ENT_SET set);
    bool addPatterns(std::unordered_map<STMT_NUM, std::string> patternMap);

 private:
    PKB &pkb;

    bool addEntityToTable(EntityTable<ENT_NAME> &table, ENT_NAME entity);
    template<typename K, typename V>
    bool addRelationshipToTable(RelationshipTable<K, V> &table, std::pair<K, V> pair);
};
