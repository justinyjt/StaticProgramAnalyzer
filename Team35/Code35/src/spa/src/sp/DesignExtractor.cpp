#include "DesignExtractor.h"

#include <memory>
#include <queue>
#include <unordered_map>
#include <utility>

#include "commons/ASTNode.h"
#include "pkb/PKBWriter.h"

DesignExtractor::DesignExtractor(std::unique_ptr<PKBWriter> pkbWriter) :
        pkbWriter_(std::move(pkbWriter)), varNameSet_(), constSet_(), procSet_(),
        stmtSet_(), readSet_(), printSet_(), assignSet_(), ifSet_(), whileSet_(),
        stmtUsePairSet_(), stmtModPairSet_(), assignPatMap_(), ifCondUsePairSet_(), whileCondUsePairSet_(),
        containerStmtLst_(), stmtCnt_(0), curProc_(), callGraph_(), CFGBuilder_(), isIfCond(),
        procDirectUseVarMap_(), procDirectModVarMap_(), containerCallPairSet_(), callProcNameToStmtMap_() {}

std::shared_ptr<ASTNode> DesignExtractor::extractProgram(std::shared_ptr<ASTNode> root) {
    root_ = std::move(root);
    for (const auto &child : root_->getChildren()) {
        extractProc(child);
    }
    if (callGraph_.isCyclic()) {
        return std::move(root_);
    }
    analyzeProcedure();
    updateStmtUsesPairSetWithCalls();
    updateStmtModsPairSetWithCalls();
    updateStmtUsesPairSetWithContainedCalls();
    updateStmtModsPairSetWithContainedCalls();

    addVarNameSetToPKB();
    addConstantSetToPKB();
    addProcSetToPKB();
    addStmtUsesPairSetToPKB();
    addStmtModifiesPairSetToPKB();
    addStmtFollowPairSetToPKB();
    addStmtParentPairSetToPKB();
    addCallProcSetToPKB();
    addReadStmtVarSetToPKB();
    addPrintStmtVarSetToPKB();
    addStmtTypesToPKB();
    addPatternsToPKB();
    addCallsToPKB();
    addCFGToPKB();
    addIfCondUsesPairSetToPKB();
    addWhileCondUsesPairSetToPKB();
    addContainerCallPairSetToPKB();
    addProcUsesPairSetToPKB();
    addProcModifiesPairSetToPKB();

    return std::move(root_);
}

void DesignExtractor::extractProc(const std::shared_ptr<ASTNode> &node) {
    const std::shared_ptr<ASTNode> &nodeC = node->getChildren().front();

    curProc_ = node->getLabel();
    procSet_.emplace(node->getLabel());

    callGraph_.addNode(curProc_);

    STMT_NUM lastCnt = stmtCnt_;
    CFGBuilder_.reset();
    CFGBuilder_.setProcName(curProc_);

    extractStmtLst(nodeC);

    CFGBuilder_.setMinStmtNum(lastCnt + 1);
    CFGBuilder_.setMaxStmtNum(stmtCnt_);

    CFGLst_.push_back(CFGBuilder_.build());
}

void DesignExtractor::extractStmtLst(const std::shared_ptr<ASTNode> &node) {
    std::unique_ptr<std::vector<STMT_NUM>> childStmtLstPtr = std::make_unique<std::vector<STMT_NUM>>();

    for (const auto &child : node->getChildren()) {
        stmtCnt_++;
        childStmtLstPtr->push_back(stmtCnt_);
        CFGBuilder_.addStmt(stmtCnt_);

        switch (child->getSyntaxType()) {
            case ASTNode::SyntaxType::Assign :
                extractAssign(child);
                break;
            case ASTNode::SyntaxType::Read :
                extractRead(child);
                break;
            case ASTNode::SyntaxType::Print :
                extractPrint(child);
                break;
            case ASTNode::SyntaxType::If :
                extractIf(child);
                break;
            case ASTNode::SyntaxType::While :
                extractWhile(child);
                break;
            case ASTNode::SyntaxType::Call :
                extractCall(child);
                break;
            default:
                break;
        }
    }

    // have children
    if (!childStmtLstPtr->empty()) {
        updateFollowsPairSet(childStmtLstPtr);
        updateParentsPairSet(childStmtLstPtr);
    }
}

void DesignExtractor::updateFollowsPairSet(const std::unique_ptr<std::vector<STMT_NUM>> &lst) {
    for (int i = 0; i < lst->size() - 1; ++i) {
        stmtFollowPairSet_.insert(STMT_STMT((*lst)[i], (*lst)[i + 1]));
        for (int j = i + 1; j < lst->size(); ++j) {
            stmtFollowStarPairSet_.insert(STMT_STMT((*lst)[i], (*lst)[j]));
        }
    }
}

void DesignExtractor::updateParentsPairSet(const std::unique_ptr<std::vector<STMT_NUM>> &lst) {
    if (containerStmtLst_.empty()) {
        return;
    }
    for (int i = 0; i < lst->size(); ++i) {
        STMT_NUM stmt = (*lst)[i];
        stmtParentPairSet_.emplace(containerStmtLst_.back(), stmt);
        for (STMT_NUM &j : containerStmtLst_) {
            stmtParentStarPairSet_.emplace(j, stmt);
        }
    }
}

void DesignExtractor::extractAssign(const std::shared_ptr<ASTNode> &node) {
    const auto &lAssign = node->getChildren().front();
    auto &rAssign = node->getChildren().back();

    ASSIGN_PAT_LEFT lAssignPat = extractLeftAssign(lAssign);
    ASSIGN_PAT_RIGHT rAssignPat = std::dynamic_pointer_cast<ExprNode>(rAssign);
    ASSIGN_PAT assignPat = std::make_pair(lAssignPat, rAssignPat);
    assignPatMap_.emplace(stmtCnt_, std::move(assignPat));
    extractRightAssign(rAssign);
}

ENT_NAME DesignExtractor::extractLeftAssign(const std::shared_ptr<ASTNode> &node) {
    const ENT_NAME &varName = node->getLabel();
    varNameSet_.emplace(varName);
    updateStmtModsPairSet(stmtCnt_, varName);
    assignSet_.emplace(stmtCnt_);
    return varName;
}

void DesignExtractor::extractRightAssign(const std::shared_ptr<ASTNode> &node) {
    const ENT_NAME &label = node->getLabel();
    switch (node->getSyntaxType()) {
        case ASTNode::SyntaxType::Variable:
            varNameSet_.emplace(label);
            updateStmtUsesPairSet(stmtCnt_, label);
            break;
        case ASTNode::SyntaxType::Constant:
            constSet_.emplace(label);
            break;
        default:
            const auto &lChild = node->getChildren().front();
            const auto &rChild = node->getChildren().back();
            extractRightAssign(lChild);
            extractRightAssign(rChild);
            break;
    }
}

void DesignExtractor::extractRead(const std::shared_ptr<ASTNode> &node) {
    const auto &child = node->getChildren().front();
    const ENT_NAME &varName = child->getLabel();
    varNameSet_.emplace(varName);
    updateStmtModsPairSet(stmtCnt_, varName);
    readSet_.emplace(stmtCnt_);
    stmtReadVarSet_.emplace(stmtCnt_, varName);
}

void DesignExtractor::extractPrint(const std::shared_ptr<ASTNode> &node) {
    const auto &child = node->getChildren().front();
    const ENT_NAME &varName = child->getLabel();
    varNameSet_.emplace(varName);
    updateStmtUsesPairSet(stmtCnt_, varName);
    printSet_.emplace(stmtCnt_);
    stmtPrintVarSet_.emplace(stmtCnt_, varName);
}

void DesignExtractor::extractCondExpr(const std::shared_ptr<ASTNode> &node) {
    const ENT_NAME &label = node->getLabel();
    switch (node->getSyntaxType()) {
        case ASTNode::SyntaxType::Variable:
            varNameSet_.emplace(label);
            updateStmtUsesPairSet(stmtCnt_, label);
            if (isIfCond) {
                ifCondUsePairSet_.emplace(stmtCnt_, label);
            } else {
                whileCondUsePairSet_.emplace(stmtCnt_, label);
            }
            break;
        case ASTNode::SyntaxType::Constant:
            constSet_.emplace(label);
            break;
        case ASTNode::SyntaxType::LogicalNot:
            extractCondExpr(node->getChildren().front());
            break;
        default:
            const auto &lChild = node->getChildren().front();
            const auto &rChild = node->getChildren().back();
            extractCondExpr(lChild);
            extractCondExpr(rChild);
            break;
    }
}

void DesignExtractor::extractIf(const std::shared_ptr<ASTNode> &node) {
    STMT_NUM ifStmtNum = stmtCnt_;
    containerStmtLst_.emplace_back(stmtCnt_);
    ifSet_.insert(stmtCnt_);

    const auto &cond = node->getChildren().at(0);
    isIfCond = true;
    extractCondExpr(cond);
    isIfCond = !isIfCond;

    const auto &thenStmtLst = node->getChildren().at(1);
    extractStmtLst(thenStmtLst);
    STMT_NUM dummyStmtNum = stmtCnt_;
    CFGBuilder_.addDummyNode(dummyStmtNum);
    CFGBuilder_.setLastVisitedStmt(ifStmtNum);

    const auto &elseStmtLst = node->getChildren().at(2);
    extractStmtLst(elseStmtLst);
    CFGBuilder_.addDummyNode(dummyStmtNum);

    containerStmtLst_.pop_back();
}

void DesignExtractor::extractWhile(const std::shared_ptr<ASTNode> &node) {
    STMT_NUM whileStmtNum = stmtCnt_;
    containerStmtLst_.push_back(stmtCnt_);
    whileSet_.insert(stmtCnt_);

    const auto &cond = node->getChildren().at(0);
    extractCondExpr(cond);

    const auto &whileStmtLst = node->getChildren().at(1);
    extractStmtLst(whileStmtLst);

    containerStmtLst_.pop_back();

    CFGBuilder_.addLoop(whileStmtNum);
}

void DesignExtractor::extractCall(const std::shared_ptr<ASTNode> &node) {
    callSet_.insert(stmtCnt_);
    const auto &child = node->getChildren().front();

    ENT_NAME calleeName = child->getLabel();
    callGraph_.addCallRelationship(curProc_, calleeName);
    stmtCallProcSet_.emplace(stmtCnt_, calleeName);
    callProcNameToStmtMap_[calleeName].insert(stmtCnt_);
    updateContainerCallPairSet(calleeName);
}

void DesignExtractor::updateStmtSet() {
    for (int i = 1; i <= stmtCnt_; ++i) {
        stmtSet_.insert(i);
    }
}

void DesignExtractor::updateStmtUsesPairSet(STMT_NUM stmt, const ENT_NAME &varName) {
    stmtUsePairSet_.emplace(stmt, varName);
    for (STMT_NUM itr : containerStmtLst_) {
        stmtUsePairSet_.emplace(itr, varName);
    }
    procDirectUseVarMap_[curProc_].insert(varName);
}

void DesignExtractor::updateStmtUsesPairSetWithCalls() {
    for (const auto &procToVarPair : procUsePairSet_) {
        const auto &procName = procToVarPair.first;
        const auto &var = procToVarPair.second;
        if (callProcNameToStmtMap_.find(procName) == callProcNameToStmtMap_.end()) {
            continue;
        }
        for (STMT_NUM stmt : callProcNameToStmtMap_.at(procName)) {
            stmtUsePairSet_.emplace(stmt, var);
        }
    }
}

void DesignExtractor::updateStmtModsPairSetWithCalls() {
    for (const auto &procToVarPair : procModPairSet_) {
        const auto &procName = procToVarPair.first;
        const auto &var = procToVarPair.second;
        if (callProcNameToStmtMap_.find(procName) == callProcNameToStmtMap_.end()) {
            continue;
        }
        for (STMT_NUM stmt : callProcNameToStmtMap_.at(procName)) {
            stmtModPairSet_.emplace(stmt, var);
        }
    }
}

void DesignExtractor::updateStmtUsesPairSetWithContainedCalls() {
    for (const auto &containerToCallStmt : containerCallPairSet_) {
        const auto &containerStmt = containerToCallStmt.first;
        const auto &procCalled = containerToCallStmt.second;
        for (const auto &procVarPair : procUsePairSet_) {
            const auto &usesProc = procVarPair.first;
            const auto &usesVar = procVarPair.second;
            if (usesProc == procCalled) {
                stmtUsePairSet_.emplace(containerStmt, usesVar);
            }
        }
    }
}

void DesignExtractor::updateStmtModsPairSetWithContainedCalls() {
    for (const auto &containerToCallStmt : containerCallPairSet_) {
        const auto &containerStmt = containerToCallStmt.first;
        const auto &procCalled = containerToCallStmt.second;
        for (const auto &procVarPair : procModPairSet_) {
            const auto &modProc = procVarPair.first;
            const auto &modVar = procVarPair.second;
            if (modProc == procCalled) {
                stmtModPairSet_.emplace(containerStmt, modVar);
            }
        }
    }
}

void DesignExtractor::updateStmtModsPairSet(STMT_NUM stmt, const ENT_NAME &varName) {
    stmtModPairSet_.emplace(stmt, varName);
    for (STMT_NUM itr : containerStmtLst_) {
        stmtModPairSet_.emplace(itr, varName);
    }
    procDirectModVarMap_[curProc_].insert(varName);
}

void DesignExtractor::updateContainerCallPairSet(const ENT_NAME &procName) {
    for (STMT_NUM itr : containerStmtLst_) {
        containerCallPairSet_.emplace(itr, procName);
    }
}

/**
 * Helper function for analyzeProcedure to recursively analyze the procedure call graph with DFS backtracking
 * @param curProcName
 * @param path
 */
void DesignExtractor::analyzeProcedureHelper(const ENT_NAME &curProcName, std::vector<ENT_NAME> &path) {
    path.emplace_back(curProcName);
    //  for every previous procedures on the path
    for (const auto &procName : path) {
        //  variables directly used by current proc
        auto iter = procDirectUseVarMap_.find(curProcName);
        if (iter != procDirectUseVarMap_.end()) {
            auto &procUseVarSet = iter->second;
            for (const auto &varUsed : procUseVarSet) {
                procUsePairSet_.emplace(procName, varUsed);
            }
        }

        //  variables directly modified by current proc
        iter = procDirectModVarMap_.find(curProcName);
        if (iter != procDirectModVarMap_.end()) {
            auto &procModVarSet = iter->second;
            for (const auto &varModified : procModVarSet) {
                procModPairSet_.emplace(procName, varModified);
            }
        }
    }
    for (const auto &proc : callGraph_.getCallees(curProcName)) {
        analyzeProcedureHelper(proc, path);
    }
    path.pop_back();
}

void DesignExtractor::analyzeProcedure() {
    for (const auto &proc : procSet_) {
        if (callGraph_.getNoOfIncomingEdges(proc)) {
            continue;
        }
        std::vector<ENT_NAME> path;
        analyzeProcedureHelper(proc, path);
    }
}

void DesignExtractor::addVarNameSetToPKB() {
    pkbWriter_->addEntities(Entity::Variable, varNameSet_);
}

void DesignExtractor::addConstantSetToPKB() {
    pkbWriter_->addEntities(Entity::Constant, constSet_);
}

void DesignExtractor::addProcSetToPKB() {
    pkbWriter_->addEntities(Entity::Procedure, procSet_);
}

void DesignExtractor::addCallProcSetToPKB() {
    pkbWriter_->addStmtEntityRelationships(StmtNameRelationship::CallsProcedure, stmtCallProcSet_);
}

void DesignExtractor::addReadStmtVarSetToPKB() {
    pkbWriter_->addStmtEntityRelationships(StmtNameRelationship::ReadStmtVar, stmtReadVarSet_);
}

void DesignExtractor::addPrintStmtVarSetToPKB() {
    pkbWriter_->addStmtEntityRelationships(StmtNameRelationship::PrintStmtVar, stmtPrintVarSet_);
}

void DesignExtractor::addStmtUsesPairSetToPKB() {
    pkbWriter_->addStmtEntityRelationships(StmtNameRelationship::Uses, stmtUsePairSet_);
}

void DesignExtractor::addStmtModifiesPairSetToPKB() {
    pkbWriter_->addStmtEntityRelationships(StmtNameRelationship::Modifies, stmtModPairSet_);
}

void DesignExtractor::addStmtFollowPairSetToPKB() {
    pkbWriter_->addStmtStmtRelationships(StmtStmtRelationship::Follows, stmtFollowPairSet_);
    pkbWriter_->addStmtStmtRelationships(StmtStmtRelationship::FollowsStar, stmtFollowStarPairSet_);
}

void DesignExtractor::addStmtParentPairSetToPKB() {
    pkbWriter_->addStmtStmtRelationships(StmtStmtRelationship::Parent, stmtParentPairSet_);
    pkbWriter_->addStmtStmtRelationships(StmtStmtRelationship::ParentStar, stmtParentStarPairSet_);
}

void DesignExtractor::addPatternsToPKB() {
    pkbWriter_->addPatterns(assignPatMap_);
}

void DesignExtractor::addStmtTypesToPKB() {
    updateStmtSet();
    pkbWriter_->addStatements(StmtType::Assign, assignSet_);
    pkbWriter_->addStatements(StmtType::Read, readSet_);
    pkbWriter_->addStatements(StmtType::Print, printSet_);
    pkbWriter_->addStatements(StmtType::If, ifSet_);
    pkbWriter_->addStatements(StmtType::While, whileSet_);
    pkbWriter_->addStatements(StmtType::Call, callSet_);
    pkbWriter_->addStatements(StmtType::None, stmtSet_);
}

std::unordered_map<STMT_NUM, ASSIGN_PAT> DesignExtractor::getAssignPatMap() {
    return this->assignPatMap_;
}

void DesignExtractor::addCallsToPKB() {
    pkbWriter_->addCallGraph(std::move(callGraph_));
}

void DesignExtractor::addCFGToPKB() {
    pkbWriter_->addCFGraphs(std::move(CFGLst_));
}

void DesignExtractor::addIfCondUsesPairSetToPKB() {
    pkbWriter_->addStmtEntityRelationships(StmtNameRelationship::IfCondVarUses, ifCondUsePairSet_);
}

void DesignExtractor::addWhileCondUsesPairSetToPKB() {
    pkbWriter_->addStmtEntityRelationships(StmtNameRelationship::WhileCondVarUses, whileCondUsePairSet_);
}

void DesignExtractor::addProcUsesPairSetToPKB() {
    pkbWriter_->addEntityEntityRelationships(NameNameRelationship::Uses, procUsePairSet_);
}

void DesignExtractor::addProcModifiesPairSetToPKB() {
    pkbWriter_->addEntityEntityRelationships(NameNameRelationship::Modifies, procModPairSet_);
}

void DesignExtractor::addContainerCallPairSetToPKB() {
    pkbWriter_->addStmtEntityRelationships(StmtNameRelationship::ContainerProcedure, containerCallPairSet_);
}
