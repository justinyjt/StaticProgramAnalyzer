#ifndef TEAM35_CODE35_SRC_SPA_SRC_COMMONS_TYPES_H_
#define TEAM35_CODE35_SRC_SPA_SRC_COMMONS_TYPES_H_

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>

#include "PairHash.h"

typedef std::string ENT_NAME;
typedef int STMT_NUM;
typedef std::uint32_t LineNumber;
typedef std::pair<STMT_NUM, ENT_NAME> STMT_ENT;
typedef std::pair<ENT_NAME , ENT_NAME> ENT_ENT;

typedef std::unordered_set<STMT_NUM> STMT_SET;
typedef std::unordered_set<ENT_NAME> ENT_SET;
typedef std::unordered_set<STMT_ENT, hash_pair> STMT_ENT_SET;
typedef std::unordered_set<ENT_ENT, hash_pair> ENT_ENT_SET;

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
