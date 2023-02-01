#ifndef TEAM35_CODE35_SRC_SPA_SRC_PKB_DB_PATTERNTABLE_H_
#define TEAM35_CODE35_SRC_SPA_SRC_PKB_DB_PATTERNTABLE_H_

#include <unordered_map>
#include <string>
#include "commons/types.h"

class PatternTable {
    std::unordered_map<STMT_NUM, std::string> stmtPatternMap;
 public:
    bool addPattern(STMT_NUM stmt, const std::string &pattern);
    STMT_SET getExactPatternMatch(const std::string &pattern) const;
};

#endif  // TEAM35_CODE35_SRC_SPA_SRC_PKB_DB_PATTERNTABLE_H_
