#pragma once

#include <optional>
#include <string>
#include <utility>

#include "commons/ASTNode.h"

class ExprNode : public ASTNode {
 public:
    static char plusAscii = '+';
    static char minusAscii = '-';
    static char multiplyAscii = '*';
    static char divideAscii = '/';
    static char moduloAscii = '%';

    ExprNode(SyntaxType syntaxType, std::optional<std::string> label)
            : ASTNode(syntaxType, std::move(label)) {}

    [[nodiscard]] std::string toString() const;

    // Override the == operator to compare two expressions
    bool operator==(const ExprNode &other) const;

    // Override the != operator to compare two expressions
    bool operator!=(const ExprNode &other) const;

    bool contains(const ExprNode &other) const;
};
