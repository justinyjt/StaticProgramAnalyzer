#include "Writer.h"

Writer::Writer(PKB &pkb) : pkb(pkb) {}

bool Writer::addEntityToTable(EntityTable<ENT_NAME> &table, ENT_NAME entity) {
    return table.addEntity(entity);
}

template<typename K, typename V>
bool Writer::addRelationshipToTable(RelationshipTable<K, V> &table, std::pair<K, V> pair) {
    return table.insertPair(pair.first, pair.second);
}
bool Writer::addEntities(Entity entity, ENT_SET entitySet) {
    EntityTable<ENT_NAME> &table = pkb.getEntityTable(entity);
    for (auto ent : entitySet) {
        if (!addEntityToTable(table, ent)) {
            return false;
        }
    }
    return true;
}

bool Writer::addStmtEntityRelationships(StmtNameRelationship tableType, STMT_ENT_SET set) {
    RelationshipTable<STMT_NUM, ENT_NAME> &table = pkb.getStmtNameRelationshipTable(tableType);
    for (auto stmtEnt : set) {
        if (!addRelationshipToTable(table, stmtEnt)) {
            return false;
        }
    }
    return true;
}

bool Writer::addPatterns(std::unordered_map<STMT_NUM, std::string> patternMap) {
    PatternTable &table = pkb.getPatternTable();
    std::unordered_map<STMT_NUM , std::string>:: iterator p;
    for (p = patternMap.begin(); p != patternMap.end(); p++) {
        if (!table.addPattern(p->first, p -> second)) {
            return false;
        }
    }
    return true;
}
