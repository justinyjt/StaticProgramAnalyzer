/**
 * 2/5/2023
 * Currently the line number is determined by the semicolomn (new semicolomn means new stat means new line)
 * need to change later
 */
#include <vector>
#include "pkb/PKB.h"
#include "DesignExtractor.h"
#include "commons/ASTNode.h"

DesignExtractor::DesignExtractor(PKB& pkb, std::unique_ptr<ASTNode> root) : pkb_(pkb), root_(std::move(root)),
                                                                            varNameSet_(), constSet_(), stmtUsePairSet_(),stmtModPairSet_(),
                                                                            assignPatMap_(), stmtCnt_(0), assignPat_() {}

void DesignExtractor::extractProgram() {
    assert(root_->getSyntaxType() == ASTNode::SyntaxType::program);
    for (const auto& child : root_->getChildren()) {
        extractProc(std::move(child));
    }
}

void DesignExtractor::extractProc(const std::unique_ptr<ASTNode>& node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::procedure);
    const std::unique_ptr<ASTNode>& nodeC = node->getChildren().front();
    assert(nodeC->getSyntaxType() == ASTNode::SyntaxType::stmtLst);
    for (const auto& child : nodeC->getChildren()) {
        stmtCnt_++;
        switch(child->getSyntaxType()){
            case ASTNode::SyntaxType::assign:
                extractAssign(child);
            default:
                break;
        }
    }
}

void DesignExtractor::extractAssign(const std::unique_ptr<ASTNode>& node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::assign);
    assignPat_.clear();
    const auto& lAssign = node->getChildren().front();
    const auto& rAssign = node->getChildren().back();
    assignPat_.append(extractLeftAssign(lAssign));
    assignPat_.append(extractRightAssign(rAssign));
    assignPatMap_.insert({stmtCnt_, assignPat_});
}

std::string DesignExtractor::extractLeftAssign(const std::unique_ptr<ASTNode>& node) {
    assert(node->getSyntaxType() == ASTNode::SyntaxType::var);
    std::string varName = node->getLabel();
    varNameSet_.insert(varName);
    stmtModPairSet_.insert(*new STMT_NAME_PAIR(stmtCnt_, varName));
    return varName + "=";
}

std::string DesignExtractor::extractRightAssign(const std::unique_ptr<ASTNode>& node) {
    std::string label = node->getLabel();
    switch(node->getSyntaxType()) {
        case ASTNode::SyntaxType::var:
            varNameSet_.insert(label);
            stmtUsePairSet_.insert(*new STMT_NAME_PAIR(stmtCnt_, label));
            return label;
        case ASTNode::SyntaxType::constVal:
            constSet_.insert(stoi(label));
            return label;
        default:
            //operators;
            //This is based on the assumption that brackets are useless in pattern matching
            //Only if we can safely ignore all brackets, this works
            assert(node->getChildren().size() == 2);
            const auto& lChild = node->getChildren().front();
            const auto& rChild = node->getChildren().back();
            return extractRightAssign(lChild) + label + extractRightAssign(rChild);
            break;
    }
}

void DesignExtractor::addVarNameSetToPKB() {
    return;
}

void DesignExtractor::addConstSetToPKB() {
    return;
}

void DesignExtractor::addStmtUsesPairSetToPKB() {
    return;
}

void DesignExtractor::addStmtModifiesPairSetToPKB() {
    return;
}

void DesignExtractor::addPatternsToPKB() {
    return;
}

std::unordered_map<STMT_NUM, std::string> DesignExtractor::getAssignPatMap() {
    return this->assignPatMap_;
}

