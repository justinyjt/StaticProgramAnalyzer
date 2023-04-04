#include <memory>

#include "PatternTable.h"
#include "commons/lexer/Lexer.h"
#include "commons/lexer/LexerFactory.h"

void PatternTable::addPattern(STMT_NUM stmt, ASSIGN_PAT pattern) {
    stmtPatternMap.emplace(stmt, pattern);
}

STMT_SET PatternTable::getExactPatternMatch(ASSIGN_PAT_RIGHT &pattern) const {
    STMT_SET result;
    for (auto &storedPattern : stmtPatternMap) {
        if (*((storedPattern.second).second) == *pattern) {
            result.emplace(storedPattern.first);
        }
    }
    return result;
}

STMT_SET PatternTable::getPartialPatternMatch(ASSIGN_PAT_RIGHT &pattern) const {
    STMT_SET result;
    for (auto &storedPattern : stmtPatternMap) {
        if ((storedPattern.second).second->contains(*pattern)) {
            result.emplace(storedPattern.first);
        }
    }
    return result;
}

STMT_ENT_SET PatternTable::getPartialPatternMatchWithLeft(ASSIGN_PAT_RIGHT &pattern) const {
    STMT_ENT_SET result;
    for (auto &storedPattern : stmtPatternMap) {
        if ((storedPattern.second).second->contains(*pattern)) {
            result.emplace(storedPattern.first, (storedPattern.second).first);
        }
    }
    return result;
}

STMT_ENT_SET PatternTable::getExactPatternMatchWithLeft(ASSIGN_PAT_RIGHT &pattern) const {
    STMT_ENT_SET result;
    for (auto &storedPattern : stmtPatternMap) {
        if (*((storedPattern.second).second) == *pattern) {
            result.emplace(storedPattern.first, (storedPattern.second).first);
        }
    }
    return result;
}

STMT_SET PatternTable::getExactPatternIntersect(ASSIGN_PAT_LEFT left, ASSIGN_PAT_RIGHT right) const {
    STMT_SET result;
    for (auto &storedPattern : stmtPatternMap) {
        if (((storedPattern.second).first) == left && *((storedPattern.second).second) == *right) {
            result.emplace(storedPattern.first);
        }
    }
    return result;
}

STMT_SET PatternTable::getPartialPatternIntersect(ASSIGN_PAT_LEFT left, ASSIGN_PAT_RIGHT right) const {
    STMT_SET result;
    for (auto &storedPattern : stmtPatternMap) {
        if (((storedPattern.second).first) == left && (storedPattern.second).second->contains(*right)) {
            result.emplace(storedPattern.first);
        }
    }
    return result;
}
