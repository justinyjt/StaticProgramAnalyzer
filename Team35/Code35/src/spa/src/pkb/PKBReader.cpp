#include "PKBReader.h"

PKBReader::PKBReader(PKB &pkb) : pkb(pkb) {}

// Statement-Entity Relationship
ENT_SET PKBReader::getEntities(Entity entityType) const {
    return pkb.getEntityTable(entityType).getTable();
}

ENT_SET PKBReader::getRelationship(StmtNameRelationship tableType, STMT_NUM stmt) const {
    return pkb.getStmtNameRelationshipTable(tableType).getValues(stmt);
}

STMT_SET PKBReader::getRelationship(StmtNameRelationship tableType, ENT_NAME name) const {
    return pkb.getStmtNameRelationshipTable(tableType).getKeys(name);
}

STMT_ENT_SET PKBReader::getAllRelationships(StmtNameRelationship tableType) const {
    return pkb.getStmtNameRelationshipTable(tableType).getKeyValuePairs();
}

bool PKBReader::isRelationshipExists(StmtNameRelationship tableType, STMT_NUM stmt, ENT_NAME name) const {
    return pkb.getStmtNameRelationshipTable(tableType).containsPair(stmt, name);
}


// Entity-Entity Relationship
ENT_SET PKBReader::getRelationshipByKey(NameNameRelationship tableType, ENT_NAME keyName) const {
    return pkb.getNameNameRelationshipTable(tableType).getValues(keyName);
}

ENT_SET PKBReader::getRelationshipByVal(NameNameRelationship tableType, ENT_NAME valName) const {
    return pkb.getNameNameRelationshipTable(tableType).getKeys(valName);
}

ENT_ENT_SET PKBReader::getAllRelationships(NameNameRelationship tableType) const {
    return pkb.getNameNameRelationshipTable(tableType).getKeyValuePairs();
}

bool PKBReader::isRelationshipExists(NameNameRelationship tableType, ENT_NAME keyName, ENT_NAME valName) const {
    return pkb.getNameNameRelationshipTable(tableType).containsPair(keyName, valName);
}

STMT_SET PKBReader::getStmtWithExactPatternMatch(std::string &pattern) const {
    return pkb.getPatternTable().getExactPatternMatch(pattern);
}
