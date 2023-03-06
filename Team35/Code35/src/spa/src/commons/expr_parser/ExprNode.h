#pragma once

#include "commons/ASTNode.h"

class ExprNode : public ASTNode {
public:
    ExprNode(SyntaxType syntaxType, std::optional<std::string> label)
            : ASTNode(syntaxType, std::move(label)) {}

    // Override the == operator to compare two expressions
    bool operator==(const ExprNode& other) const {
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

    // Override the != operator to compare two expressions
    bool operator!=(const ExprNode& other) const {
        return !(*this == other);
    }
};



