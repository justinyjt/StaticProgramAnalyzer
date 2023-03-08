#include <memory>

#include "PatternTable.h"
#include "commons/lexer/Lexer.h"
#include "commons/lexer/LexerFactory.h"

bool PatternTable::addPattern(STMT_NUM stmt, ASSIGN_PAT &&pattern) {
    stmtPatternMap.insert({stmt, std::move(pattern)});
    return true;
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
