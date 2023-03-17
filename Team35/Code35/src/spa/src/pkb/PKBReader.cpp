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

bool PKBReader::isRelationshipExists(NameNameRelationship tableType, ENT_NAME keyName, ENT_NAME valName) const {
    return pkb.isEntEntPairExists(tableType, keyName, valName);
}

// Statement-Statement Relationship
STMT_SET PKBReader::getRelationshipByKey(StmtStmtRelationship tableType, STMT_NUM keyName) const {
    switch (tableType) {
        case StmtStmtRelationship::AffectsStar:
        case StmtStmtRelationship::Affects:
            return this->getAffectsByPredecessor(keyName);
        default:
            return pkb.getStmtByStmtKey(tableType, keyName);
    }
}

STMT_SET PKBReader::getRelationshipByVal(StmtStmtRelationship tableType, STMT_NUM valName) const {
    switch (tableType) {
        case StmtStmtRelationship::AffectsStar:
        case StmtStmtRelationship::Affects:
            return this->getAffectsBySuccessor(valName);
        default:
            return pkb.getStmtByStmtVal(tableType, valName);
    }
}

STMT_STMT_SET PKBReader::getAllRelationships(StmtStmtRelationship tableType) {
    switch (tableType) {
        case StmtStmtRelationship::AffectsStar:
        case StmtStmtRelationship::Affects:
            return this->getAllAffects();
        default:
            return pkb.getStmtStmtSet(tableType);
    }
}

STMT_SET PKBReader::getKeyStmtByRelationship(StmtStmtRelationship tableType) const {
    switch (tableType) {
        case StmtStmtRelationship::AffectsStar:
        case StmtStmtRelationship::Affects:
            return this->getAllAffectsPredecessors();
        default:
            return pkb.getKeyStmtByRs(tableType);
    }
}

STMT_SET PKBReader::getValueStmtByRelationship(StmtStmtRelationship tableType) const {
    switch (tableType) {
        case StmtStmtRelationship::AffectsStar:
        case StmtStmtRelationship::Affects:
            return this->getAllAffectsSuccessors();
        default:
            return pkb.getValStmtByRs(tableType);
    }
}

STMT_SET PKBReader::getStmtByProc(const ENT_NAME &procName) const {
    return pkb.getStmtByProc(procName);
}

ENT_SET PKBReader::getKeyNameByRelationship(NameNameRelationship tableType) const {
    return pkb.getKeyNameByRs(tableType);
}

ENT_SET PKBReader::getValueNameByRelationship(NameNameRelationship tableType) const {
    return pkb.getValNameByRs(tableType);
}

bool PKBReader::isRelationshipExists(StmtStmtRelationship tableType, STMT_NUM keyName, STMT_NUM valName) const {
    switch (tableType) {
        case StmtStmtRelationship::AffectsStar:
        case StmtStmtRelationship::Affects:
            return this->isAffects(keyName, valName);
        default:
            return pkb.isStmtStmtPairExists(tableType, keyName, valName);
    }
}

STMT_SET PKBReader::getStmtWithExactPatternMatch(ASSIGN_PAT_RIGHT &pattern) const {
    return pkb.getPatternTable().getExactPatternMatch(pattern);
}

STMT_SET PKBReader::getStmtWithPartialPatternMatch(ASSIGN_PAT_RIGHT &pattern) const {
    return pkb.getPatternTable().getPartialPatternMatch(pattern);
}

bool PKBReader::isAffects(STMT_NUM stmt1, STMT_NUM stmt2) const {
    if (!pkb.isEntityTypeExists(StmtType::Assign, stmt1) ||
        !pkb.isEntityTypeExists(StmtType::Assign, stmt2)) {
        return false;
    }
    ENT_SET modifies = pkb.getEntByStmtKey(StmtNameRelationship::Modifies, stmt1);
    ENT_SET uses = pkb.getEntByStmtKey(StmtNameRelationship::Uses, stmt2);

    for (auto &modifiedEnt : modifies) {
        if (uses.find(modifiedEnt) == uses.end()) {
            return false;
        }
    }
    if (!pkb.isStmtStmtPairExists(StmtStmtRelationship::NextStar, stmt1, stmt2)) {
        return false;
    }
    STMT_SET successors = pkb.getStmtByStmtKey(StmtStmtRelationship::NextStar, stmt1);
    STMT_SET predecessors = pkb.getStmtByStmtVal(StmtStmtRelationship::NextStar, stmt2);
    bool hasIntersection = false;
    for (auto &successor : successors) {
        if (predecessors.find(successor) == predecessors.end()) {
            continue;
        }
        for (auto &modifiedEnt : modifies) {
            if (isModifies(successor, modifiedEnt)) {
                return false;
            }
        }
        hasIntersection = true;
    }
    return hasIntersection;
}

bool PKBReader::isModifies(STMT_NUM key, const ENT_NAME &val) const {
    ENT_SET invalidModifies = pkb.getEntByStmtKey(StmtNameRelationship::Modifies, key);
    if (invalidModifies.find(val) != invalidModifies.end()) {
        return true;
    }
    ENT_SET calls = pkb.getEntByStmtKey(StmtNameRelationship::Calls, key);
    if (calls.empty()) {
        return false;
    }
    bool isCallModifies = calls.find(val) != calls.end();
    return isCallModifies;
}

bool PKBReader::isValidAffectsSuccessor(STMT_NUM stmt) const {
    if (!pkb.isEntityTypeExists(StmtType::Assign, stmt)) {
        return false;
    }
    STMT_SET predecessors = pkb.getStmtByStmtVal(StmtStmtRelationship::NextStar, stmt);
    for (auto &predecessor : predecessors) {
        if (isAffects(predecessor, stmt)) {
            return true;
        }
    }
    return false;
}

bool PKBReader::isValidAffectsPredecessor(STMT_NUM stmt) const {
    if (!pkb.isEntityTypeExists(StmtType::Assign, stmt)) {
        return false;
    }
    ENT_SET uses = pkb.getEntByStmtKey(StmtNameRelationship::Uses, stmt);
    if (uses.empty()) {
        return false;
    }
    STMT_SET successors = pkb.getStmtByStmtKey(StmtStmtRelationship::NextStar, stmt);
    for (auto &successor : successors) {
        if (isAffects(stmt, successor)) {
            return true;
        }
    }
    return false;
}

STMT_SET PKBReader::getAffectsByPredecessor(STMT_NUM stmt) const {
    if (!pkb.isEntityTypeExists(StmtType::Assign, stmt)) {
        return {};
    }
    ENT_SET uses = pkb.getEntByStmtKey(StmtNameRelationship::Uses, stmt);
    if (uses.empty()) {
        return {};
    }
    STMT_SET successors = pkb.getStmtByStmtKey(StmtStmtRelationship::NextStar, stmt);
    STMT_SET affectsSuccessors;
    for (auto &successor : successors) {
        if (isAffects(successor, stmt)) {
            affectsSuccessors.insert(successor);
        }
    }
    return affectsSuccessors;
}


STMT_SET PKBReader::getAffectsBySuccessor(STMT_NUM stmt) const {
    if (!pkb.isEntityTypeExists(StmtType::Assign, stmt)) {
        return {};
    }
    STMT_SET predecessors = pkb.getStmtByStmtVal(StmtStmtRelationship::NextStar, stmt);
    STMT_SET affectsPredecessors;
    for (auto &predecessor : predecessors) {
        if (isAffects(predecessor, stmt)) {
            affectsPredecessors.insert(predecessor);
        }
    }
    return affectsPredecessors;
}

STMT_STMT_SET PKBReader::getAllAffects() const {
    STMT_STMT_SET result;
    for (auto &predecessor : pkb.getKeyStmtByRs(StmtStmtRelationship::NextStar)) {
        for (auto &successor : this->getAffectsByPredecessor(predecessor)) {
            result.emplace(predecessor, successor);
        }
    }
    return result;
}

STMT_SET PKBReader::getAllAffectsPredecessors() const {
    STMT_SET result;
    for (auto &predecessor : pkb.getKeyStmtByRs(StmtStmtRelationship::NextStar)) {
        if (this->isValidAffectsPredecessor(predecessor)) {
            result.emplace(predecessor);
        }
    }
    return result;
}

STMT_SET PKBReader::getAllAffectsSuccessors() const {
    STMT_SET result;
    for (auto &successor : pkb.getValStmtByRs(StmtStmtRelationship::NextStar)) {
        if (this->isValidAffectsSuccessor(successor)) {
            result.emplace(successor);
        }
    }
    return result;
}

bool PKBReader::hasAffects() const {
    for (auto &predecessor : pkb.getKeyStmtByRs(StmtStmtRelationship::NextStar)) {
        if (this->isValidAffectsPredecessor(predecessor)) {
            return true;
        }
    }
    return false;
}
