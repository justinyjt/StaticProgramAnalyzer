#include "ExprNode.h"

std::string ExprNode::toString() const {
    return this->getLabel();
}

bool ExprNode::operator==(const ExprNode &other) const {
    if (getSyntaxType() != other.getSyntaxType()) {
        return false;
    }

    if (getLabel() != other.getLabel()) {
        return false;
    }

    if (getChildren().size() != other.getChildren().size()) {
        return false;
    }

    for (size_t i = 0; i < getChildren().size(); ++i) {
        auto thisChild = dynamic_cast<const ExprNode*>(getChildren()[i].get());
        auto otherChild = dynamic_cast<const ExprNode*>(other.getChildren()[i].get());

        if (thisChild == nullptr || otherChild == nullptr || *thisChild != *otherChild) {
            return false;
        }
    }

    return true;
}

bool ExprNode::operator!=(const ExprNode& other) const {
    return !(*this == other);
}
