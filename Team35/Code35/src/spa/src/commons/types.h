#ifndef TEAM35_CODE35_SRC_SPA_SRC_COMMONS_TYPES_H_
#define TEAM35_CODE35_SRC_SPA_SRC_COMMONS_TYPES_H_

#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "PairHash.h"
#include "commons/expr_parser/ExprNode.h"

typedef std::string ENT_NAME;
typedef int STMT_NUM;
typedef std::uint32_t LineNumber;
typedef std::pair<STMT_NUM, ENT_NAME> STMT_ENT;
typedef std::pair<ENT_NAME, ENT_NAME> ENT_ENT;
typedef std::pair<STMT_NUM, STMT_NUM> STMT_STMT;

typedef std::unordered_set<STMT_NUM> STMT_SET;
typedef std::unordered_set<ENT_NAME> ENT_SET;
typedef std::unordered_set<STMT_ENT, hash_pair> STMT_ENT_SET;
typedef std::unordered_set<ENT_ENT, hash_pair> ENT_ENT_SET;
typedef std::unordered_set<STMT_STMT, hash_pair> STMT_STMT_SET;

typedef std::shared_ptr<ExprNode> ASSIGN_PAT_RIGHT;
typedef ENT_NAME ASSIGN_PAT_LEFT;
typedef std::pair<ASSIGN_PAT_LEFT, ASSIGN_PAT_RIGHT> ASSIGN_PAT;

enum class StmtType {
    Print, Read, Assign, Call, While, If, None
};

enum class EntType {
    Procedure, Variable, Constant
};

enum class StmtNameRelationship {
    Modifies, Uses, Calls, Reads, Prints, None
};

enum class NameNameRelationship {
    Uses, Modifies, Calls, CallsStar, None
};

enum class StmtStmtRelationship {
    Parent, ParentStar, Follows, FollowsStar, None
};

enum class Entity {
    Variable, Constant, Procedure
};

#endif  // TEAM35_CODE35_SRC_SPA_SRC_COMMONS_TYPES_H_
