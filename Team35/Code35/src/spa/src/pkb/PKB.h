#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "pkb/db/RelationshipTable.h"
#include "commons/types.h"



typedef int PROC;

class TNode;

class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class PKB {
 public:
    static VarTable* varTable;
    static int setProcToAST(PROC p, TNode* r);
    static TNode* getRootAST(PROC p);

    RelationshipTable<STMT_NUM, ENT_NAME> modifiesStmtNameTable;
    RelationshipTable<STMT_NUM, ENT_NAME> usesStmtNameTable;
    RelationshipTable<STMT_NUM, ENT_NAME> callsStmtNameTable;
    RelationshipTable<STMT_NUM, ENT_NAME> readsStmtNameTable;
    RelationshipTable<STMT_NUM, ENT_NAME> printsStmtNameTable;
    bool checkRelationshipExists(StmtNameRelationship tableType);
//    const RelationshipTable<STMT_NUM, ENT_NAME> &getStmtNameRelationshipTable(StmtNameRelationship tableType) const;
};
