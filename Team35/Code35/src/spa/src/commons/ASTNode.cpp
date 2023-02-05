#include "ASTNode.h"

#include <utility>

ASTNode::ASTNode(SyntaxType syntaxType, std::optional<std::string> label) {
    this->type_ = syntaxType;
    this->label_ = std::move(label);
}

ASTNode::SyntaxType ASTNode::getSyntaxType() const {
    return this->type_;
}

std::string ASTNode::getLabel() const {
    return this->label_.value();
}

void ASTNode::addChild(std::unique_ptr<ASTNode> newChild) {
    this->children_.push_back(std::move(newChild));
}

const std::vector<std::unique_ptr<ASTNode> >& ASTNode::getChildren() const {
    return this->children_;
}
