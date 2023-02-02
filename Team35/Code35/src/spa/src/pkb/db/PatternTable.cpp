#include "PatternTable.h"

bool PatternTable::addPattern(STMT_NUM stmt, std::string pattern) {
    stmtPatternMap.insert({stmt, pattern});
    return true;
}

STMT_SET PatternTable::getExactPatternMatch(std::string pattern) const {
    STMT_SET result;

    for (auto storedPattern : stmtPatternMap) {
        if (storedPattern.second == pattern) {
            result.emplace(storedPattern.first);
        }
    }
    return result;
}
