#include <cassert>
#include <memory>
#include <queue>
#include <utility>
#include <unordered_map>

#include "pkb/PKBWriter.h"
#include "DesignExtractor.h"
#include "commons/ASTNode.h"

struct ProcNode {
    ENT_NAME procName;
    std::vector<ENT_NAME> path;

    ProcNode(const ENT_NAME &name, const std::vector<ENT_NAME> &newPath)
            : procName(name), path(newPath) {}
};


DesignExtractor::DesignExtractor(std::unique_ptr<PKBWriter> pkbWriter) :
        pkbWriter_(std::move(pkbWriter)), varNameSet_(), constSet_(), procSet_(),
        stmtSet_(), readSet_(), printSet_(), assignSet_(), ifSet_(), whileSet_(),
        callSet_(), stmtUsePairSet_(), stmtModPairSet_(), stmtCallProcSet_(),
        stmtReadVarSet_(), stmtPrintVarSet_(), assignPatMap_(), ifCondUsePairSet_(), whileCondUsePairSet_(),
        containerStmtLst_(), stmtCnt_(0), curProc_(), callGraph_(), CFGBuilder_(), isIfCond(),
        procDirectUseVarMap_(), procDirectModVarMap_(), isProcGetCalled_(), containerCallPairSet_() {}

std::shared_ptr<ASTNode> DesignExtractor::extractProgram(std::shared_ptr<ASTNode> root) {
    root_ = std::move(root);
    assert(root_->getSyntaxType() == ASTNode::SyntaxType::Program);
    for (const auto &child: root_->getChildren()) {
        extractProc(child);
    }
    if (callGraph_.isCyclic()) {
        return std::move(root_);
    }
    analyzeProc();

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
    assert(node->getSyntaxType() == ASTNode::SyntaxType::Procedure);
    const std::shared_ptr<ASTNode> &nodeC = node->getChildren().front();

    curProc_ = node->getLabel();
    procSet_.insert(node->getLabel());

    callGraph_.addNode(curProc_);

    int lastCnt = stmtCnt_;
    CFGBuilder_.reset();
    CFGBuilder_.setProcName(curProc_);

    extractStmtLst(nodeC);

    CFGBuilder_.setMinStmtNum(lastCnt + 1);
    CFGBuilder_.setMaxStmtNum(stmtCnt_);

    CFGLst_.push_back(CFGBuilder_.build());
}

void DesignExtractor::extractStmtLst(const std::shared_ptr<ASTNode> &node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::StmtLst);
    std::unique_ptr<std::vector<STMT_NUM>> childStmtLstPtr = std::make_unique<std::vector<STMT_NUM>>();

    for (const auto &child: node->getChildren()) {
        stmtCnt_++;
        childStmtLstPtr->push_back(stmtCnt_);
        CFGBuilder_.addStmt(stmtCnt_);

        switch (child->getSyntaxType()) {
            case ASTNode::SyntaxType::Assign:
                extractAssign(child);
                break;
            case ASTNode::SyntaxType::Read:
                extractRead(child);
                break;
            case ASTNode::SyntaxType::Print:
                extractPrint(child);
                break;
            case ASTNode::SyntaxType::If:
                extractIf(child);
                break;
            case ASTNode::SyntaxType::While:
                extractWhile(child);
                break;
            case ASTNode::SyntaxType::Call:
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
    for (int i = 0; i < lst->size(); i++) {
        STMT_NUM stmt = (*lst)[i];
        stmtParentPairSet_.insert(STMT_STMT(containerStmtLst_.back(), stmt));
        for (int &j: containerStmtLst_) {
            stmtParentStarPairSet_.insert(STMT_STMT(j, stmt));
        }
    }
}

void DesignExtractor::extractAssign(const std::shared_ptr<ASTNode> &node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::Assign);
    const auto &lAssign = node->getChildren().front();
    auto &rAssign = node->getChildren().back();

    ASSIGN_PAT_LEFT lAssignPat = extractLeftAssign(lAssign);
    ASSIGN_PAT_RIGHT rAssignPat = std::dynamic_pointer_cast<ExprNode>(rAssign);
    ASSIGN_PAT assignPat = std::make_pair(lAssignPat, rAssignPat);
    assignPatMap_.emplace(stmtCnt_, std::move(assignPat));
    extractRightAssign(rAssign);
}

ENT_NAME DesignExtractor::extractLeftAssign(const std::shared_ptr<ASTNode> &node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::Variable);
    const std::string &varName = node->getLabel();
    varNameSet_.insert(varName);
    updateStmtModsPairSet(stmtCnt_, varName);
    assignSet_.insert(stmtCnt_);
    return varName;
}

void DesignExtractor::extractRightAssign(const std::shared_ptr<ASTNode> &node) {
    const std::string &label = node->getLabel();
    switch (node->getSyntaxType()) {
        case ASTNode::SyntaxType::Variable:
            varNameSet_.insert(label);
            updateStmtUsesPairSet(stmtCnt_, label);
            return;
        case ASTNode::SyntaxType::Constant:
            constSet_.insert(label);
            return;
        default:
            assert(node->getChildren().size() == 2);
            const auto &lChild = node->getChildren().front();
            const auto &rChild = node->getChildren().back();
            extractRightAssign(lChild);
            extractRightAssign(rChild);
            return;
    }
}

void DesignExtractor::extractRead(const std::shared_ptr<ASTNode> &node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::Read);
    const auto &child = node->getChildren().front();
    assert(child->getSyntaxType() == ASTNode::SyntaxType::Variable);
    const std::string &varName = child->getLabel();
    varNameSet_.insert(varName);
    updateStmtModsPairSet(stmtCnt_, varName);
    readSet_.insert(stmtCnt_);
    stmtReadVarSet_.emplace(stmtCnt_, varName);
}

void DesignExtractor::extractPrint(const std::shared_ptr<ASTNode> &node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::Print);
    const auto &child = node->getChildren().front();
    assert(child->getSyntaxType() == ASTNode::SyntaxType::Variable);
    const std::string &varName = child->getLabel();
    varNameSet_.insert(varName);
    updateStmtUsesPairSet(stmtCnt_, varName);
    printSet_.insert(stmtCnt_);
    stmtPrintVarSet_.emplace(stmtCnt_, varName);
}

void DesignExtractor::extractCondExpr(const std::shared_ptr<ASTNode> &node) {
    const std::string &label = node->getLabel();
    switch (node->getSyntaxType()) {
        case ASTNode::SyntaxType::Variable:
            varNameSet_.insert(label);
            updateStmtUsesPairSet(stmtCnt_, label);
            if (isIfCond) {
                ifCondUsePairSet_.emplace(stmtCnt_, label);
            } else {
                whileCondUsePairSet_.emplace(stmtCnt_, label);
            }
            break;
        case ASTNode::SyntaxType::Constant:
            constSet_.insert(label);
            break;
        case ASTNode::SyntaxType::LogicalNot:
            assert(node->getChildren().size() == 1);
            extractCondExpr(node->getChildren().front());
            break;
        default:
            assert(node->getChildren().size() == 2);
            const auto &lChild = node->getChildren().front();
            const auto &rChild = node->getChildren().back();
            extractCondExpr(lChild);
            extractCondExpr(rChild);
            break;
    }
}

void DesignExtractor::extractIf(const std::shared_ptr<ASTNode> &node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::If);

    STMT_NUM ifStmtNum = stmtCnt_;
    containerStmtLst_.push_back(stmtCnt_);
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
    assert(node->getSyntaxType() == ASTNode::SyntaxType::While);

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
    assert(node->getSyntaxType() == ASTNode::SyntaxType::Call);
    callSet_.insert(stmtCnt_);
    const auto &child = node->getChildren().front();
    assert(child->getSyntaxType() == ASTNode::SyntaxType::Variable);

    ENT_NAME calleeName = child->getLabel();
    callGraph_.addCallRelationship(curProc_, calleeName);
    stmtCallProcSet_.emplace(stmtCnt_, calleeName);
    isProcGetCalled_[calleeName] = true;
    updateContainerCallPairSet(calleeName);
}

void DesignExtractor::updateStmtSet() {
    for (int i = 1; i <= stmtCnt_; ++i) {
        stmtSet_.insert(i);
    }
}

void DesignExtractor::updateStmtUsesPairSet(STMT_NUM stmt, const ENT_NAME &varName) {
    stmtUsePairSet_.insert(STMT_ENT(stmt, varName));
    for (STMT_NUM itr: containerStmtLst_) {
        stmtUsePairSet_.insert(STMT_ENT(itr, varName));
    }
    procDirectUseVarMap_[curProc_].insert(varName);
}

void DesignExtractor::updateStmtModsPairSet(STMT_NUM stmt, const ENT_NAME &varName) {
    stmtModPairSet_.insert(STMT_ENT(stmt, varName));
    for (STMT_NUM itr: containerStmtLst_) {
        stmtModPairSet_.insert(STMT_ENT(itr, varName));
    }
    procDirectModVarMap_[curProc_].insert(varName);
}

void DesignExtractor::updateContainerCallPairSet(const ENT_NAME &procName) {
    for (STMT_NUM itr: containerStmtLst_) {
        containerCallPairSet_.insert(STMT_ENT(itr, procName));
    }
}

void DesignExtractor::analyzeProc() {
    std::queue<ProcNode> procQueue;
    for (const auto &proc: procSet_) {
        if (!isProcGetCalled_[proc]) {
            procQueue.push(ProcNode(proc, std::vector<ENT_NAME>()));

            while (!procQueue.empty()) {
                auto &curProcNode = procQueue.front();
                auto &curProcName = curProcNode.procName;
                auto &curPath = curProcNode.path;
                auto newPath = std::vector<ENT_NAME>(curPath);
                newPath.push_back(curProcName);

                //  For every previous procs on the path
                for (const auto &procName: newPath) {
                    //  Variables directly used by current proc
                    auto iter = procDirectUseVarMap_.find(curProcName);
                    if (iter != procDirectUseVarMap_.end()) {
                        auto &procUseVarSet = iter->second;
                        for (const auto &varUsed: procUseVarSet) {
                            procUsePairSet_.insert(ENT_ENT(procName, varUsed));
                        }
                    }
                    //  Variables directly modified by current proc
                    iter = procDirectModVarMap_.find(curProcName);
                    if (iter != procDirectModVarMap_.end()) {
                        auto &procModVarSet = iter->second;
                        for (const auto &varModed: procModVarSet) {
                            procModPairSet_.insert(ENT_ENT(procName, varModed));
                        }
                    }
                }
                for (const auto &calleeProc: callGraph_.getCallingProcs(curProcName)) {
                    procQueue.push(ProcNode(calleeProc, newPath));
                }
                procQueue.pop();
            }
        }
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
