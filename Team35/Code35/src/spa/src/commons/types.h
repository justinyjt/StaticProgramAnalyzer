#ifndef TEAM35_CODE35_SRC_SPA_SRC_COMMONS_TYPES_H_
#define TEAM35_CODE35_SRC_SPA_SRC_COMMONS_TYPES_H_

#include <string>

typedef  std::string ENT_NAME;
typedef int STMT_NUM;


enum class StmtNameRelationship {
    Modifies, Uses, Calls, Reads, Prints, None
};

#endif  // TEAM35_CODE35_SRC_SPA_SRC_COMMONS_TYPES_H_
