#pragma once

#include <string>
#include <utility>

#include "commons/ASTNode.h"

class ExprNode : public ASTNode {
 public:
    ExprNode(SyntaxType syntaxType, std::optional<std::string> label)
            : ASTNode(syntaxType, std::move(label)) {}

    [[nodiscard]] std::string toString() const;

    // Override the == operator to compare two expressions
    bool operator==(const ExprNode &other) const;

    // Override the != operator to compare two expressions
    bool operator!=(const ExprNode &other) const;

    bool contains(const ExprNode &other) const;
};