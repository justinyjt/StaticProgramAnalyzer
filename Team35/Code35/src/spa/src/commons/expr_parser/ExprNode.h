#pragma once

#include <string>
#include <utility>

#include "commons/ASTNode.h"

class ExprNode : public ASTNode {
 public:
    static std::string plusLabel;
    static std::string minusLabel;
    static std::string multiplyLabel;
    static std::string divideLabel;
    static std::string moduloLabel;

    ExprNode(SyntaxType syntaxType, std::optional<std::string> label)
            : ASTNode(syntaxType, std::move(label)) {}

    [[nodiscard]] std::string toString() const;

    // Override the == operator to compare two expressions
    bool operator==(const ExprNode &other) const;

    // Override the != operator to compare two expressions
    bool operator!=(const ExprNode &other) const;

    bool contains(const ExprNode &other) const;
};
