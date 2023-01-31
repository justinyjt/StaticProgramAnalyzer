#ifndef SPA_TYPES_H
#define SPA_TYPES_H

#include <string>

typedef  std::string ENT_NAME;
typedef int STMT_NUM;


enum class StmtNameRelationship {
    Modifies, Uses, Calls, Reads, Prints, None
};
#endif //SPA_TYPES_H
