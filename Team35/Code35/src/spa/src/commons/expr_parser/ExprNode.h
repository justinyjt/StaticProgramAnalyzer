#pragma once

#include <optional>
#include <string>
#include <utility>

#include "commons/ASTNode.h"

class ExprNode : public ASTNode {
 public:
    static const char plusAscii = '+';
    static const char minusAscii = '-';
    static const char multiplyAscii = '*';
    static const char divideAscii = '/';
    static const char moduloAscii = '%';

    ExprNode(SyntaxType syntaxType, std::optional<std::string> label)
            : ASTNode(syntaxType, std::move(label)) {}

    [[nodiscard]] std::string toString() const override;

    // Override the == operator to compare two expressions
    bool operator==(const ExprNode &other) const;

    // Override the != operator to compare two expressions
    bool operator!=(const ExprNode &other) const;

    bool contains(const ExprNode &other) const;
};
