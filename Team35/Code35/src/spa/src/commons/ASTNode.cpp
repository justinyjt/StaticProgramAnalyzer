#include "ASTNode.h"

#include <utility>

ASTNode::ASTNode(SyntaxType syntaxType, std::optional<std::string> label) {
    this->type = syntaxType;
    this->label = std::move(label);
}

ASTNode::SyntaxType ASTNode::getSyntaxType() const {
    return this->type;
}

std::string ASTNode::getLabel() const {
    return this->label.value();
}

void ASTNode::addChild(const std::shared_ptr<ASTNode> &newChild) {
    this->children.push_back(newChild);
}
