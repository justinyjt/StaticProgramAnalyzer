#include "PKBReader.h"

PKBReader::PKBReader(PKB &pkb) : pkb(pkb) {}

ENT_SET PKBReader::getEntities(Entity entityType) const {
    return pkb.getEntityTable(entityType).getTable();
}

STMT_SET PKBReader::getStatements(StmtType stmtType) const {
    return pkb.getStatementTable(stmtType).getTable();
}

// Statement-Entity Relationship
ENT_SET PKBReader::getRelationship(StmtNameRelationship tableType, STMT_NUM stmt) const {
    return pkb.getEntByStmtKey(tableType, stmt);
}

STMT_SET PKBReader::getRelationship(StmtNameRelationship tableType, ENT_NAME name) const {
    return pkb.getStmtByEntVal(tableType, name);
}

STMT_ENT_SET PKBReader::getAllRelationships(StmtNameRelationship tableType) const {
    return pkb.getStmtEntSet(tableType);
}

STMT_SET PKBReader::getStmtByRelationship(StmtNameRelationship tableType) const {
    return pkb.getStmtByRs(tableType);
}

bool PKBReader::isRelationshipExists(StmtNameRelationship tableType, STMT_NUM stmt, ENT_NAME name) const {
    return pkb.isStmtEntPairExists(tableType, stmt, name);
}


// Entity-Entity Relationship
ENT_SET PKBReader::getRelationshipByKey(NameNameRelationship tableType, ENT_NAME keyName) const {
    return pkb.getEntByEntKey(tableType, keyName);
}

ENT_SET PKBReader::getRelationshipByVal(NameNameRelationship tableType, ENT_NAME valName) const {
    return pkb.getEntByEntVal(tableType, valName);
}

ENT_ENT_SET PKBReader::getAllRelationships(NameNameRelationship tableType) const {
    return pkb.getEntEntSet(tableType);
}

ENT_SET PKBReader::getKeys(NameNameRelationship tableType) const {
    return pkb.getKeysFrom(tableType);
}

bool PKBReader::isRelationshipExists(NameNameRelationship tableType, ENT_NAME keyName, ENT_NAME valName) const {
    return pkb.isEntEntPairExists(tableType, keyName, valName);
}

// Statement-Statement Relationship
STMT_SET PKBReader::getRelationshipByKey(StmtStmtRelationship tableType, STMT_NUM keyName) const {
    return pkb.getStmtByStmtKey(tableType, keyName);
}

STMT_SET PKBReader::getRelationshipByVal(StmtStmtRelationship tableType, STMT_NUM valName) const {
    return pkb.getStmtByStmtVal(tableType, valName);
}

STMT_STMT_SET PKBReader::getAllRelationships(StmtStmtRelationship tableType) const {
    return pkb.getStmtStmtSet(tableType);
}

STMT_SET PKBReader::getKeyStmtByRelationship(StmtStmtRelationship tableType) const {
    return pkb.getKeyStmtByRs(tableType);
}

STMT_SET PKBReader::getValueStmtByRelationship(StmtStmtRelationship tableType) const {
    return pkb.getValStmtByRs(tableType);
}


bool PKBReader::isRelationshipExists(StmtStmtRelationship tableType, STMT_NUM keyName, STMT_NUM valName) const {
    return pkb.isStmtStmtPairExists(tableType, keyName, valName);
}

STMT_SET PKBReader::getStmtWithExactPatternMatch(ASSIGN_PAT_RIGHT &pattern) const {
    return pkb.getPatternTable().getExactPatternMatch(pattern);
}

STMT_SET PKBReader::getStmtWithPartialPatternMatch(ASSIGN_PAT_RIGHT &pattern) const {
    return pkb.getPatternTable().getPartialPatternMatch(pattern);
}

