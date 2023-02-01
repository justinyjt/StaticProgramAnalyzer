#ifndef TEAM35_CODE35_SRC_SPA_SRC_COMMONS_TYPES_H_
#define TEAM35_CODE35_SRC_SPA_SRC_COMMONS_TYPES_H_

#include <string>
#include <unordered_set>

typedef  std::string ENT_NAME;
typedef int STMT_NUM;
typedef std::unordered_set<STMT_NUM> STMT_SET;


enum class StmtNameRelationship {
    Modifies, Uses, Calls, Reads, Prints, None
};

enum class NameNameRelationship {
    Uses, Modifies, None
};

enum class Entity {
    Variable, Constant
};

#endif  // TEAM35_CODE35_SRC_SPA_SRC_COMMONS_TYPES_H_
