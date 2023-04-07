#include <memory>
#include <queue>
#include <string>
#include <unordered_set>
#include <utility>

#include "PairHash.h"
#include "commons/expr_parser/ExprNode.h"

typedef std::string ENT_NAME;
typedef int STMT_NUM;
typedef std::pair<STMT_NUM, ENT_NAME> STMT_ENT;
typedef std::pair<ENT_NAME, ENT_NAME> ENT_ENT;
typedef std::pair<STMT_NUM, STMT_NUM> STMT_STMT;

typedef std::unordered_set<STMT_NUM> STMT_SET;
typedef std::unordered_set<ENT_NAME> ENT_SET;
typedef std::unordered_set<STMT_ENT, hash_pair> STMT_ENT_SET;
typedef std::unordered_set<ENT_ENT, hash_pair> ENT_ENT_SET;
typedef std::unordered_set<STMT_STMT, hash_pair> STMT_STMT_SET;
typedef std::queue<STMT_NUM> STMT_QUEUE;

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
    Modifies,
    Uses,
    Calls,
    IfCondVarUses,
    WhileCondVarUses,
    ContainerProcedure,
    CallsProcedure,
    ReadStmtVar,
    PrintStmtVar
};

enum class NameNameRelationship {
    Uses, Modifies, Calls, CallsStar
};

enum class StmtStmtRelationship {
    Parent, ParentStar, Follows, FollowsStar, Next, NextStar, Affects, AffectsStar
};

enum class Entity {
    Variable, Constant, Procedure
};
