#include "PKBReader.h"

#include <algorithm>

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

STMT_SET PKBReader::getRelationship(StmtNameRelationship tableType, const ENT_NAME &name) const {
    return pkb.getStmtByEntVal(tableType, name);
}

STMT_ENT_SET PKBReader::getAllRelationships(StmtNameRelationship tableType) const {
    return pkb.getStmtEntSet(tableType);
}

STMT_SET PKBReader::getStmtByRelationship(StmtNameRelationship tableType) const {
    return pkb.getStmtByRs(tableType);
}

bool PKBReader::isRelationshipExists(StmtNameRelationship tableType, STMT_NUM stmt, const ENT_NAME &name) const {
    return pkb.isStmtEntPairExists(tableType, stmt, name);
}

// Entity-Entity Relationship
ENT_SET PKBReader::getRelationshipByKey(NameNameRelationship tableType, const ENT_NAME &keyName) const {
    return pkb.getEntByEntKey(tableType, keyName);
}

ENT_SET PKBReader::getRelationshipByVal(NameNameRelationship tableType, const ENT_NAME &valName) const {
    return pkb.getEntByEntVal(tableType, valName);
}

ENT_ENT_SET PKBReader::getAllRelationships(NameNameRelationship tableType) const {
    return pkb.getEntEntSet(tableType);
}

bool PKBReader::isRelationshipExists(NameNameRelationship tableType, const ENT_NAME &keyName,
                                     const ENT_NAME &valName) const {
    return pkb.isEntEntPairExists(tableType, keyName, valName);
}

// Statement-Statement Relationship
STMT_SET PKBReader::getRelationshipByKey(StmtStmtRelationship tableType, STMT_NUM keyName) {
    switch (tableType) {
        case StmtStmtRelationship::AffectsStar:
            return this->getAffectsByPredecessor(keyName, true);
        case StmtStmtRelationship::Affects:
            return this->getAffectsByPredecessor(keyName, false);
        default:
            return pkb.getStmtByStmtKey(tableType, keyName);
    }
}

STMT_SET PKBReader::getRelationshipByVal(StmtStmtRelationship tableType, STMT_NUM valName) {
    switch (tableType) {
        case StmtStmtRelationship::AffectsStar:
            return this->getAffectsBySuccessor(valName, true);
        case StmtStmtRelationship::Affects:
            return this->getAffectsBySuccessor(valName, false);
        default:
            return pkb.getStmtByStmtVal(tableType, valName);
    }
}

STMT_STMT_SET PKBReader::getAllRelationships(StmtStmtRelationship tableType) {
    switch (tableType) {
        case StmtStmtRelationship::AffectsStar:
            return this->getAllAffects(true);
        case StmtStmtRelationship::Affects:
            return this->getAllAffects(false);
        default:
            return pkb.getStmtStmtSet(tableType);
    }
}

STMT_SET PKBReader::getKeyStmtByRelationship(StmtStmtRelationship tableType) {
    switch (tableType) {
        case StmtStmtRelationship::AffectsStar:
        case StmtStmtRelationship::Affects:
            return this->getAllAffectsPredecessors();
        default:
            return pkb.getKeyStmtByRs(tableType);
    }
}

STMT_SET PKBReader::getValueStmtByRelationship(StmtStmtRelationship tableType) {
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

bool PKBReader::isRelationshipExists(StmtStmtRelationship tableType, STMT_NUM keyName, STMT_NUM valName) {
    switch (tableType) {
        case StmtStmtRelationship::AffectsStar:
            return this->isAffectsT(keyName, valName);
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

bool PKBReader::isAffects(STMT_NUM stmt1, STMT_NUM stmt2) {
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
    STMT_SET successors = pkb.getStmtByStmtKey(StmtStmtRelationship::Next, stmt1);
    STMT_QUEUE frontier;
    for (auto &successor : successors) {
        frontier.push(successor);
    }
    STMT_SET visited;
    while (!frontier.empty()) {
        STMT_NUM num = frontier.front();
        frontier.pop();
        if (visited.find(num) != visited.end()) {
            continue;
        }
        visited.insert(num);
        if (num == stmt2) {
            affects_graph_.addAffectsEdge(stmt1, stmt2);
            return true;
        }
        if (pkb.isEntityTypeExists(StmtType::While, num)
            || pkb.isEntityTypeExists(StmtType::If, num)) {
            STMT_SET curr = pkb.getStmtByStmtKey(StmtStmtRelationship::Next, num);
            for (auto &currSuccessor : curr) {
                frontier.push(currSuccessor);
            }
            continue;
        }
        bool isModified = false;
        for (auto &modifiedEnt : modifies) {
            if (isModifies(num, modifiedEnt)) {
                isModified = true;
            }
        }
        if (isModified) {
            continue;
        }
        STMT_SET curr = pkb.getStmtByStmtKey(StmtStmtRelationship::Next, num);
        for (auto &currSuccessor : curr) {
            frontier.push(currSuccessor);
        }
    }
    return false;
}

bool PKBReader::isAffectsT(STMT_NUM first, STMT_NUM second) {
    if (!pkb.isEntityTypeExists(StmtType::Assign, first) ||
        !pkb.isEntityTypeExists(StmtType::Assign, second)) {
        return false;
    }
    if (!pkb.isStmtStmtPairExists(StmtStmtRelationship::NextStar, first, second)) {
        return false;
    }
    if (affects_graph_.isEdgeExist(first, second)) {
        return true;
    }
    STMT_SET intersect = getIntersect(first, second);
    for (auto &stmt1 : intersect) {
        for (auto &stmt2 : intersect) {
            if (affects_graph_.isEdgeExist(stmt1, stmt2)) {
                continue;
            }
            isAffects(stmt1, stmt2);
        }
    }
    return affects_graph_.isEdgeExist(first, second);
}

STMT_SET PKBReader::getIntersect(STMT_NUM first, STMT_NUM second) const {
    STMT_SET successors = pkb.getStmtByStmtKey(StmtStmtRelationship::NextStar, first);
    STMT_SET predecessors = pkb.getStmtByStmtVal(StmtStmtRelationship::NextStar, second);
    STMT_SET intersect;
    for (auto &successor : successors) {
        if (predecessors.find(successor) != predecessors.end()) {
            intersect.emplace(successor);
        }
    }
    intersect.emplace(first);
    intersect.emplace(second);
    return intersect;
}

bool PKBReader::isModifies(STMT_NUM key, const ENT_NAME &val) const {
    ENT_SET invalidModifies = pkb.getEntByStmtKey(StmtNameRelationship::Modifies, key);
    if (invalidModifies.find(val) != invalidModifies.end()) {
        return true;
    }
    ENT_SET calls = pkb.getEntByStmtKey(StmtNameRelationship::CallsProcedure, key);
    if (calls.empty()) {
        return false;
    }
    bool isCallModifies = calls.find(val) != calls.end();
    return isCallModifies;
}

bool PKBReader::isValidAffectsSuccessor(STMT_NUM stmt) {
    if (!pkb.isEntityTypeExists(StmtType::Assign, stmt)) {
        return false;
    }
    ENT_SET uses = pkb.getEntByStmtKey(StmtNameRelationship::Uses, stmt);
    if (uses.empty()) {
        return {};
    }
    STMT_SET predecessors = pkb.getStmtByStmtVal(StmtStmtRelationship::NextStar, stmt);
    return std::any_of(predecessors.begin(), predecessors.end(),
                       [stmt, this](auto &predecessor) {
                         return isAffects(predecessor, stmt);
                       });
}

bool PKBReader::isValidAffectsPredecessor(STMT_NUM stmt) {
    if (!pkb.isEntityTypeExists(StmtType::Assign, stmt)) {
        return false;
    }
    STMT_SET successors = pkb.getStmtByStmtKey(StmtStmtRelationship::NextStar, stmt);
    return std::any_of(successors.begin(), successors.end(),
                       [stmt, this](auto &successor) {
                         return isAffects(stmt, successor);
                       });
}

STMT_SET PKBReader::getAffectsByPredecessor(STMT_NUM stmt, bool isTransitive) {
    if (!pkb.isEntityTypeExists(StmtType::Assign, stmt)) {
        return {};
    }
    STMT_SET successors = pkb.getStmtByStmtKey(StmtStmtRelationship::NextStar, stmt);
    STMT_SET affectsSuccessors;
    if (!isTransitive) {
        for (auto &successor : successors) {
            if (isAffects(stmt, successor)) {
                affectsSuccessors.insert(successor);
            }
        }
    } else {
        for (auto &successor : successors) {
            if (isAffectsT(stmt, successor)) {
                affectsSuccessors.insert(successor);
            }
        }
    }
    return affectsSuccessors;
}

STMT_SET PKBReader::getAffectsBySuccessor(STMT_NUM stmt, bool isTransitive) {
    if (!pkb.isEntityTypeExists(StmtType::Assign, stmt)) {
        return {};
    }
    STMT_SET predecessors = pkb.getStmtByStmtVal(StmtStmtRelationship::NextStar, stmt);
    STMT_SET affectsPredecessors;
    if (!isTransitive) {
        for (auto &predecessor : predecessors) {
            if (isAffects(predecessor, stmt)) {
                affectsPredecessors.insert(predecessor);
            }
        }
    } else {
        for (auto &predecessor : predecessors) {
            if (isAffectsT(predecessor, stmt)) {
                affectsPredecessors.insert(predecessor);
            }
        }
    }
    return affectsPredecessors;
}

STMT_STMT_SET PKBReader::getAllAffects(bool isTransitive) {
    STMT_STMT_SET result;
    for (auto &predecessor : pkb.getKeyStmtByRs(StmtStmtRelationship::NextStar)) {
        for (auto &successor : this->getAffectsByPredecessor(predecessor, isTransitive)) {
            result.emplace(predecessor, successor);
        }
    }
    return result;
}

STMT_SET PKBReader::getAllAffectsPredecessors() {
    STMT_SET result;
    for (auto &predecessor : pkb.getKeyStmtByRs(StmtStmtRelationship::NextStar)) {
        if (this->isValidAffectsPredecessor(predecessor)) {
            result.emplace(predecessor);
        }
    }
    return result;
}

STMT_SET PKBReader::getAllAffectsSuccessors() {
    STMT_SET result;
    for (auto &successor : pkb.getValStmtByRs(StmtStmtRelationship::NextStar)) {
        if (this->isValidAffectsSuccessor(successor)) {
            result.emplace(successor);
        }
    }
    return result;
}

bool PKBReader::hasAffects() {
    const auto &nextStar = pkb.getKeyStmtByRs(StmtStmtRelationship::NextStar);
    return std::any_of(nextStar.begin(), nextStar.end(), [this](auto &predecessor) {
      return this->isValidAffectsPredecessor(predecessor);
    });
}
