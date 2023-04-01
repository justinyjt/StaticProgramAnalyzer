#include "ExprNode.h"

#include <string>

std::string ExprNode::toString() const {
    if (this->getChildren().size() > 0) {
        std::string label;
        auto firstChild = dynamic_cast<const ExprNode *>(getChildren()[0].get());
        label += firstChild->toString();
        label += this->getLabel();
        auto secondChild = dynamic_cast<const ExprNode *>(getChildren()[1].get());
        label += secondChild->toString();
        return label;
    } else {
        return this->getLabel();
    }
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
        auto thisChild = dynamic_cast<const ExprNode *>(getChildren()[i].get());
        auto otherChild = dynamic_cast<const ExprNode *>(other.getChildren()[i].get());

        if (thisChild == nullptr || otherChild == nullptr || *thisChild != *otherChild) {
            return false;
        }
    }

    return true;
}

bool ExprNode::operator!=(const ExprNode &other) const {
    return !(*this == other);
}

bool ExprNode::contains(const ExprNode &other) const {
    if (*this == other) {
        return true;
    }
    for (size_t i = 0; i < this->getChildren().size(); ++i) {
        auto thisChild = dynamic_cast<const ExprNode *>(getChildren()[i].get());
        if (thisChild->contains(other)) {
            return true;
        }
    }
    return false;
}
