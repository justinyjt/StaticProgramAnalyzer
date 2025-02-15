#include "PKBReader.h"

#include <algorithm>

#include "commons/util/SetUtil.h"

PKBReader::PKBReader(PKB &pkb) : pkb(pkb), isAffectsComputed(false) {}

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

STMT_SET
PKBReader::getRelationshipWithFilter(StmtNameRelationship tableType, const ENT_NAME &name, StmtType stmtType) const {
    STMT_SET s = this->getRelationship(tableType, name);
    STMT_SET filterSet = this->getStatements(stmtType);
    return SetUtil::intersection(s, filterSet);
}

STMT_ENT_SET PKBReader::getAllRelationships(StmtNameRelationship tableType) const {
    return pkb.getStmtEntSet(tableType);
}

STMT_ENT_SET PKBReader::getAllRelationshipsWithFilter(StmtNameRelationship tableType, StmtType stmtType) const {
    STMT_ENT_SET s = this->getAllRelationships(tableType);
    STMT_SET filterSet = this->getStatements(stmtType);
    STMT_ENT_SET result;
    for (auto const &stmtEntPair : s) {
        auto &stmt = stmtEntPair.first;
        if (filterSet.find(stmt) != filterSet.end()) {
            result.emplace(stmt, stmtEntPair.second);
        }
    }
    return result;
}

STMT_SET PKBReader::getStmtByRelationship(StmtNameRelationship tableType) const {
    return pkb.getStmtByRs(tableType);
}

STMT_SET PKBReader::getStmtByRelationshipWithFilter(StmtNameRelationship tableType, StmtType stmtType) const {
    STMT_SET parentStmtSet = this->getStmtByRelationship(tableType);
    STMT_SET filterSet = this->getStatements(stmtType);
    return SetUtil::intersection(parentStmtSet, filterSet);
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
            return this->getAffectsByPredecessor(keyName, UsageType::Transitive);
        case StmtStmtRelationship::Affects:
            return this->getAffectsByPredecessor(keyName, UsageType::Direct);
        default:
            return pkb.getStmtByStmtKey(tableType, keyName);
    }
}

STMT_SET PKBReader::getRelationshipByVal(StmtStmtRelationship tableType, STMT_NUM valName) {
    switch (tableType) {
        case StmtStmtRelationship::AffectsStar:
            return this->getAffectsBySuccessor(valName, UsageType::Transitive);
        case StmtStmtRelationship::Affects:
            return this->getAffectsBySuccessor(valName, UsageType::Direct);
        default:
            return pkb.getStmtByStmtVal(tableType, valName);
    }
}

STMT_SET PKBReader::getRelationshipByStmtWithFilter(StmtStmtRelationship tableType, STMT_NUM stmt, StmtType stmtType,
                                                    ArgType argType) {
    STMT_SET parentStmtSet = argType == ArgType::Key
                             ? this->getRelationshipByKey(tableType, stmt)
                             : this->getRelationshipByVal(tableType, stmt);
    STMT_SET filterSet = this->getStatements(stmtType);
    return SetUtil::intersection(parentStmtSet, filterSet);
}

STMT_STMT_SET PKBReader::getAllRelationships(StmtStmtRelationship tableType) {
    switch (tableType) {
        case StmtStmtRelationship::AffectsStar:
            return this->getAllAffects(UsageType::Transitive);
        case StmtStmtRelationship::Affects:
            return this->getAllAffects(UsageType::Direct);
        default:
            return pkb.getStmtStmtSet(tableType);
    }
}

STMT_STMT_SET PKBReader::getAllRelationshipsWithFilter(StmtStmtRelationship tableType, StmtType stmtType) {
    STMT_STMT_SET s = this->getAllRelationships(tableType);
    STMT_SET filterSet = this->getStatements(stmtType);
    STMT_STMT_SET result;
    for (auto const &stmtStmtPair : s) {
        auto &stmt1 = stmtStmtPair.first;
        auto &stmt2 = stmtStmtPair.second;
        if (stmt1 == stmt2 && filterSet.find(stmt1) != filterSet.end()) {
            result.emplace(stmt1, stmt2);
        }
    }
    return result;
}

STMT_STMT_SET PKBReader::getAllRelationshipsWithFilter(StmtStmtRelationship tableType,
                                                       StmtType first,
                                                       StmtType second) {
    STMT_STMT_SET s = this->getAllRelationships(tableType);
    STMT_SET filterFirst = this->getStatements(first);
    STMT_SET filterSecond = this->getStatements(second);
    STMT_STMT_SET result;
    for (auto const &stmtStmtPair : s) {
        auto &stmt1 = stmtStmtPair.first;
        auto &stmt2 = stmtStmtPair.second;
        if (filterFirst.find(stmt1) != filterFirst.end() && filterSecond.find(stmt2) != filterSecond.end()) {
            result.emplace(stmt1, stmt2);
        }
    }
    return result;
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

bool PKBReader::hasRelationship(StmtStmtRelationship tableType) {
    switch (tableType) {
        case StmtStmtRelationship::AffectsStar:
        case StmtStmtRelationship::Affects:
            return this->hasAffects();
        default:
            return pkb.isRelationshipExists(tableType);
    }
}

STMT_SET PKBReader::getStmtByRelationshipWithFilter(StmtStmtRelationship tableType,
                                                    StmtType stmtType,
                                                    ArgType argType) {
    STMT_SET parentStmtSet = argType == ArgType::Key
                             ? this->getKeyStmtByRelationship(tableType)
                             : this->getValueStmtByRelationship(tableType);
    STMT_SET filterSet = this->getStatements(stmtType);
    return SetUtil::intersection(parentStmtSet, filterSet);
}

STMT_SET PKBReader::getStmtWithExactPatternMatch(ASSIGN_PAT_RIGHT &pattern) const {
    return pkb.getStmtWithExactPatternMatch(pattern);
}

STMT_SET PKBReader::getStmtWithPartialPatternMatch(ASSIGN_PAT_RIGHT &pattern) const {
    return pkb.getStmtWithPartialPatternMatch(pattern);
}

STMT_ENT_SET PKBReader::getStmtVarExactPatternMatch(ASSIGN_PAT_RIGHT &pattern) const {
    return pkb.getStmtVarWithExactPatternMatch(pattern);
}

STMT_ENT_SET PKBReader::getStmtVarPartialPatternMatch(ASSIGN_PAT_RIGHT &pattern) const {
    return pkb.getStmtVarWithPartialPatternMatch(pattern);
}

STMT_SET PKBReader::getStmtWithExactPatternIntersect(ASSIGN_PAT_LEFT left, ASSIGN_PAT_RIGHT &right) const {
    return pkb.getStmtIntersectWithExactPatternMatch(left, right);
}

STMT_SET PKBReader::getStmtWithPartialPatternIntersect(ASSIGN_PAT_LEFT left, ASSIGN_PAT_RIGHT &right) const {
    return pkb.getStmtIntersectWithPartialPatternMatch(left, right);
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
    return isAffectsHelper(stmt1, stmt2, modifies);
}

bool PKBReader::isAffectsT(STMT_NUM first, STMT_NUM second) {
    if (!pkb.isEntityTypeExists(StmtType::Assign, first) ||
        !pkb.isEntityTypeExists(StmtType::Assign, second)) {
        return false;
    }
    if (!pkb.isStmtStmtPairExists(StmtStmtRelationship::NextStar, first, second)) {
        return false;
    }
    if (affects_graph_.hasAffectsRelationship(first, second)) {
        return true;
    }
    if (isAffectsComputed) {
        return affects_graph_.isReachable(first, second, UsageType::Transitive);
    }
    STMT_SET intersect = getIntersect(first, second);
    for (auto &stmt1 : intersect) {
        for (auto &stmt2 : intersect) {
            if (affects_graph_.hasAffectsRelationship(stmt1, stmt2)) {
                continue;
            }
            if (isAffects(stmt1, stmt2)) {
                affects_graph_.addAffectsEdge(stmt1, stmt2);
            }
        }
    }
    return affects_graph_.isReachable(first, second, UsageType::Transitive);
}

STMT_SET PKBReader::getIntersect(STMT_NUM first, STMT_NUM second) {
    STMT_SET successors = this->getRelationshipByStmtWithFilter(StmtStmtRelationship::NextStar, first, StmtType::Assign,
                                                                ArgType::Key);
    STMT_SET predecessors = this->getRelationshipByStmtWithFilter(StmtStmtRelationship::NextStar, second,
                                                                  StmtType::Assign,
                                                                  ArgType::Value);
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

bool PKBReader::isContainerStmt(STMT_NUM num) const {
    return pkb.isEntityTypeExists(StmtType::While, num)
           || pkb.isEntityTypeExists(StmtType::If, num);
}

bool PKBReader::isSuccessorCandidate(STMT_NUM num, ENT_SET modifies) const {
    for (auto &modifiedEnt : modifies) {
        if (isModifies(num, modifiedEnt)) {
            return false;
        }
    }
    return true;
}

bool PKBReader::isAffectsHelper(STMT_NUM start, STMT_NUM end, ENT_SET modifies) const {
    STMT_SET successors = pkb.getStmtByStmtKey(StmtStmtRelationship::Next, start);
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
        if (num == end) {
            return true;
        }
        if (!isContainerStmt(num) && !isSuccessorCandidate(num, modifies)) {
            continue;
        }
        STMT_SET curr = pkb.getStmtByStmtKey(StmtStmtRelationship::Next, num);
        for (auto &currSuccessor : curr) {
            frontier.push(currSuccessor);
        }
    }
    return false;
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

STMT_SET PKBReader::getAffectsByPredecessor(STMT_NUM stmt1, UsageType usageType) {
    if (!pkb.isEntityTypeExists(StmtType::Assign, stmt1)) {
        return {};
    }
    STMT_SET successors = this->getRelationshipByStmtWithFilter(StmtStmtRelationship::NextStar, stmt1, StmtType::Assign,
                                                                ArgType::Key);
    if (usageType == UsageType::Direct) {
        STMT_SET affectsSuccessors;
        for (auto &successor : successors) {
            if (isAffects(stmt1, successor)) {
                affectsSuccessors.insert(successor);
            }
        }
        return affectsSuccessors;
    } else {
        this->computeAffectsGraph();
        return affects_graph_.getSuccessors(stmt1, UsageType::Transitive);
    }
}

STMT_SET PKBReader::getAffectsBySuccessor(STMT_NUM stmt2, UsageType usageType) {
    if (!pkb.isEntityTypeExists(StmtType::Assign, stmt2)) {
        return {};
    }
    STMT_SET predecessors = this->getRelationshipByStmtWithFilter(StmtStmtRelationship::NextStar, stmt2,
                                                                  StmtType::Assign, ArgType::Value);

    if (usageType == UsageType::Direct) {
        STMT_SET affectsPredecessors;
        for (auto &predecessor : predecessors) {
            if (isAffects(predecessor, stmt2)) {
                affectsPredecessors.insert(predecessor);
            }
        }
        return affectsPredecessors;
    } else {
        this->computeAffectsGraph();
        return affects_graph_.getPredecessors(stmt2, UsageType::Transitive);
    }
}

STMT_STMT_SET PKBReader::getAllAffects(UsageType usageType) {
    this->computeAffectsGraph();
    return affects_graph_.getAllAffectsRelationships(usageType);
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

void PKBReader::computeAffectsGraph() {
    if (isAffectsComputed) {
        return;
    }
    for (auto &predecessor : this->getStatements(StmtType::Assign)) {
        STMT_SET successors = this->getRelationshipByStmtWithFilter(StmtStmtRelationship::NextStar, predecessor,
                                                                    StmtType::Assign, ArgType::Key);
        for (auto &successor : successors) {
            if (isAffects(predecessor, successor)) {
                affects_graph_.addAffectsEdge(predecessor, successor);
            }
        }
    }
    isAffectsComputed = true;
}

void PKBReader::clearCache() {
    affects_graph_.reset();
    isAffectsComputed = false;
}
