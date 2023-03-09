#ifndef TEAM35_CODE35_SRC_SPA_SRC_PKB_DB_PATTERNTABLE_H_
#define TEAM35_CODE35_SRC_SPA_SRC_PKB_DB_PATTERNTABLE_H_

#include <unordered_map>
#include <string>
#include "commons/types.h"

class PatternTable {
    std::unordered_map<STMT_NUM, ASSIGN_PAT> stmtPatternMap;
 public:
    bool addPattern(STMT_NUM stmt, ASSIGN_PAT pattern);

    STMT_SET getExactPatternMatch(ASSIGN_PAT_RIGHT &pattern) const;

    STMT_SET getPartialPatternMatch(ASSIGN_PAT_RIGHT &pattern) const;
};

#endif  // TEAM35_CODE35_SRC_SPA_SRC_PKB_DB_PATTERNTABLE_H_
