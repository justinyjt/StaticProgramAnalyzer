#ifndef TEAM35_CODE35_SRC_SPA_SRC_PKB_DB_PATTERNTABLE_H_
#define TEAM35_CODE35_SRC_SPA_SRC_PKB_DB_PATTERNTABLE_H_

#include <unordered_map>
#include <string>
#include "commons/types.h"

class PatternTable {
    std::unordered_map<STMT_NUM, ASSIGN_PAT> stmtPatternMap;
 public:
    void addPattern(STMT_NUM stmt, ASSIGN_PAT pattern);

    STMT_SET getExactPatternMatch(ASSIGN_PAT_RIGHT &pattern) const;

    STMT_SET getPartialPatternMatch(ASSIGN_PAT_RIGHT &pattern) const;

    STMT_ENT_SET getPartialPatternMatchWithLeft(ASSIGN_PAT_RIGHT &pattern) const;

    STMT_ENT_SET getExactPatternMatchWithLeft(ASSIGN_PAT_RIGHT &pattern) const;

    STMT_SET getExactPatternIntersect(ASSIGN_PAT_LEFT left, ASSIGN_PAT_RIGHT right) const;

    STMT_SET getPartialPatternIntersect(ASSIGN_PAT_LEFT left, ASSIGN_PAT_RIGHT right) const;
};

#endif  // TEAM35_CODE35_SRC_SPA_SRC_PKB_DB_PATTERNTABLE_H_
