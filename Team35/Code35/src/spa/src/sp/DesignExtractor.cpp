/**
 * 2/5/2023
 * Currently the line number is determined by the semicolomn (new semicolomn means new stat means new line)
 * need to change later
 */
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
    stmtUsePairSet_(), stmtModPairSet_(), assignPatMap_(), stmtCnt_(0), assignPat_() {}

std::unique_ptr<ASTNode> DesignExtractor::extractProgram(std::unique_ptr<ASTNode> root) {
    root_ = std::move(root);
    assert(root_->getSyntaxType() == ASTNode::SyntaxType::program);
    for (const auto &child : root_->getChildren()) {
        extractProc(std::move(child));
    }
    addVarNameSetToPKB();
    addStmtUsesPairSetToPKB();
    addStmtModifiesPairSetToPKB();
    addPatternsToPKB();
    return std::move(root_);
}

void DesignExtractor::extractProc(const std::unique_ptr<ASTNode> &node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::procedure);
    const std::unique_ptr<ASTNode> &nodeC = node->getChildren().front();
    assert(nodeC->getSyntaxType() == ASTNode::SyntaxType::stmtLst);
    for (const auto &child : nodeC->getChildren()) {
        stmtCnt_++;
        switch (child->getSyntaxType()) {
            case ASTNode::SyntaxType::assign:extractAssign(child);
                break;
            case ASTNode::SyntaxType::read:extractRead(child);
                break;
            case ASTNode::SyntaxType::print:extractPrint(child);
                break;
            default:break;
        }
    }
}

void DesignExtractor::extractAssign(const std::unique_ptr<ASTNode> &node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::assign);
    assignPat_.clear();
    const auto &lAssign = node->getChildren().front();
    const auto &rAssign = node->getChildren().back();
    extractLeftAssign(lAssign);
    assignPat_.append(extractRightAssign(rAssign));
    assignPatMap_.insert({stmtCnt_, assignPat_});
}

std::string DesignExtractor::extractLeftAssign(const std::unique_ptr<ASTNode> &node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::var);
    std::string varName = node->getLabel();
    varNameSet_.insert(varName);
    stmtModPairSet_.insert(STMT_ENT(stmtCnt_, varName));
    return varName + "=";
}

std::string DesignExtractor::extractRightAssign(const std::unique_ptr<ASTNode> &node) {
    std::string label = node->getLabel();
    switch (node->getSyntaxType()) {
        case ASTNode::SyntaxType::var:varNameSet_.insert(label);
            stmtUsePairSet_.insert(STMT_ENT(stmtCnt_, label));
            return label;
        case ASTNode::SyntaxType::constVal:constSet_.insert(stmtCnt_);
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
    assert(node->getSyntaxType() == ASTNode::SyntaxType::read);
    const auto &child = node->getChildren().front();
    assert(child->getSyntaxType() == ASTNode::SyntaxType::var);
    std::string varName = child->getLabel();
    varNameSet_.insert(varName);
    stmtModPairSet_.insert(STMT_ENT(stmtCnt_, varName));
}

void DesignExtractor::extractPrint(const std::unique_ptr<ASTNode> &node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::print);
    const auto &child = node->getChildren().front();
    assert(child->getSyntaxType() == ASTNode::SyntaxType::var);
    std::string varName = child->getLabel();
    varNameSet_.insert(varName);
    stmtUsePairSet_.insert(STMT_ENT(stmtCnt_, varName));
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

void DesignExtractor::addPatternsToPKB() {
    pkbWriter_->addPatterns(assignPatMap_);
}

std::unordered_map<STMT_NUM, std::string> DesignExtractor::getAssignPatMap() {
    return this->assignPatMap_;
}

