#include <cassert>
#include <memory>
#include <string>
#include <utility>
#include <unordered_map>

#include "pkb/PKBWriter.h"
#include "DesignExtractor.h"
#include "commons/ASTNode.h"

DesignExtractor::DesignExtractor(std::unique_ptr<PKBWriter> pkbWriter) :
    pkbWriter_(std::move(pkbWriter)), varNameSet_(), constSet_(),
    stmtSet_(), readSet_(), printSet_(), assignSet_(), ifSet_(), whileSet_(),
    stmtUsePairSet_(), stmtModPairSet_(), assignPatMap_(),
    containerStmtLst_(), stmtCnt_(0), assignPat_() {}

std::unique_ptr<ASTNode> DesignExtractor::extractProgram(std::unique_ptr<ASTNode> root) {
    root_ = std::move(root);
    assert(root_->getSyntaxType() == ASTNode::SyntaxType::Program);
    for (const auto &child : root_->getChildren()) {
        extractProc(std::move(child));
    }
    addVarNameSetToPKB();
    addStmtUsesPairSetToPKB();
    addStmtModifiesPairSetToPKB();
    addStmtFollowPairSetToPKB();
    addStmtParentPairSetToPKB();
    addPatternsToPKB();
    addStmtTypesToPKB();
    return std::move(root_);
}

void DesignExtractor::extractProc(const std::unique_ptr<ASTNode> &node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::Procedure);
    const std::unique_ptr<ASTNode> &nodeC = node->getChildren().front();
    extractStmtLst(nodeC);
}

void DesignExtractor::extractStmtLst(const std::unique_ptr<ASTNode> &node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::StmtLst);
    std::unique_ptr<std::vector<STMT_NUM>> childStmtLstPtr = std::make_unique<std::vector<STMT_NUM>>();

    for (const auto &child : node->getChildren()) {
        stmtCnt_++;
        childStmtLstPtr->push_back(stmtCnt_);
        switch (child->getSyntaxType()) {
            case ASTNode::SyntaxType::Assign:extractAssign(child);
                break;
            case ASTNode::SyntaxType::Read:extractRead(child);
                break;
            case ASTNode::SyntaxType::Print:extractPrint(child);
                break;
            case ASTNode::SyntaxType::If:extractIf(child);
                break;
            case ASTNode::SyntaxType::While:extractWhile(child);
                break;
            default:break;
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
        for (int &j : containerStmtLst_) {
            stmtParentStarPairSet_.insert(STMT_STMT(j, stmt));
        }
    }
}

void DesignExtractor::extractAssign(const std::unique_ptr<ASTNode> &node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::Assign);
    assignPat_.clear();
    const auto &lAssign = node->getChildren().front();
    const auto &rAssign = node->getChildren().back();
    extractLeftAssign(lAssign);
    assignPat_.append(extractRightAssign(rAssign));
    assignPatMap_.insert({stmtCnt_, assignPat_});
}

std::string DesignExtractor::extractLeftAssign(const std::unique_ptr<ASTNode> &node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::Variable);
    std::string varName = node->getLabel();
    varNameSet_.insert(varName);
    updateStmtModsPairSet(stmtCnt_, varName);
    assignSet_.insert(stmtCnt_);
    return varName + "=";
}

std::string DesignExtractor::extractRightAssign(const std::unique_ptr<ASTNode> &node) {
    std::string label = node->getLabel();
    switch (node->getSyntaxType()) {
        case ASTNode::SyntaxType::Variable:varNameSet_.insert(label);
            updateStmtUsesPairSet(stmtCnt_, label);
            return label;
        case ASTNode::SyntaxType::Constant:constSet_.insert(stmtCnt_);
            return label;
        default:
            // operators;
            // This is based on the assumption that brackets are useless in pattern matching
            // Only if we can safely ignore all brackets, this works
            assert(node->getChildren().size() == 2);
            const auto &lChild = node->getChildren().front();
            const auto &rChild = node->getChildren().back();
            return extractRightAssign(lChild) + label + extractRightAssign(rChild);
            break;
    }
}

void DesignExtractor::extractRead(const std::unique_ptr<ASTNode> &node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::Read);
    const auto &child = node->getChildren().front();
    assert(child->getSyntaxType() == ASTNode::SyntaxType::Variable);
    std::string varName = child->getLabel();
    varNameSet_.insert(varName);
    updateStmtModsPairSet(stmtCnt_, varName);
    readSet_.insert(stmtCnt_);
}

void DesignExtractor::extractPrint(const std::unique_ptr<ASTNode> &node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::Print);
    const auto &child = node->getChildren().front();
    assert(child->getSyntaxType() == ASTNode::SyntaxType::Variable);
    std::string varName = child->getLabel();
    varNameSet_.insert(varName);
    updateStmtUsesPairSet(stmtCnt_, varName);
    printSet_.insert(stmtCnt_);
}

void DesignExtractor::extractCondExpr(const std::unique_ptr<ASTNode> &node) {
    std::string label = node->getLabel();
    switch (node->getSyntaxType()) {
        case ASTNode::SyntaxType::Variable:varNameSet_.insert(label);
            updateStmtUsesPairSet(stmtCnt_, label);
            break;
        case ASTNode::SyntaxType::Constant:constSet_.insert(stmtCnt_);
            break;
        case ASTNode::SyntaxType::LogicalNot:assert(node->getChildren().size() == 1);
            extractCondExpr(node->getChildren().front());
            break;
        default:
            // operators;
            // This is based on the assumption that brackets are useless in pattern matching
            // Only if we can safely ignore all brackets, this works
            assert(node->getChildren().size() == 2);
            const auto &lChild = node->getChildren().front();
            const auto &rChild = node->getChildren().back();
            extractCondExpr(lChild);
            extractCondExpr(rChild);
            break;
    }
}

void DesignExtractor::extractIf(const std::unique_ptr<ASTNode> &node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::If);
    containerStmtLst_.push_back(stmtCnt_);
    ifSet_.insert(stmtCnt_);

    const auto &cond = node->getChildren().at(0);
    extractCondExpr(cond);

    const auto &thenStmtLst = node->getChildren().at(1);
    extractStmtLst(thenStmtLst);

    const auto &elseStmtLst = node->getChildren().at(2);
    extractStmtLst(elseStmtLst);

    containerStmtLst_.pop_back();
}

void DesignExtractor::extractWhile(const std::unique_ptr<ASTNode> &node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::While);
    containerStmtLst_.push_back(stmtCnt_);
    whileSet_.insert(stmtCnt_);

    const auto &cond = node->getChildren().at(0);
    extractCondExpr(cond);

    const auto &whileStmtLst = node->getChildren().at(1);
    extractStmtLst(whileStmtLst);

    containerStmtLst_.pop_back();
}

void DesignExtractor::updateStmtSet() {
    for (int i = 1; i <= stmtCnt_; ++i) {
        stmtSet_.insert(i);
    }
}

void DesignExtractor::updateStmtUsesPairSet(STMT_NUM stmt, std::string varName) {
    stmtUsePairSet_.insert(STMT_ENT(stmt, varName));
    for (STMT_NUM itr : containerStmtLst_) {
        stmtUsePairSet_.insert(STMT_ENT(itr, varName));
    }
}

void DesignExtractor::updateStmtModsPairSet(STMT_NUM stmt, std::string varName) {
    stmtModPairSet_.insert(STMT_ENT(stmt, varName));
    for (STMT_NUM itr : containerStmtLst_) {
        stmtModPairSet_.insert(STMT_ENT(itr, varName));
    }
}

void DesignExtractor::addVarNameSetToPKB() {
    pkbWriter_->addEntities(Entity::Variable, varNameSet_);
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
    pkbWriter_->addStatements(StmtType::None, stmtSet_);
}

std::unordered_map<STMT_NUM, std::string> DesignExtractor::getAssignPatMap() {
    return this->assignPatMap_;
}
