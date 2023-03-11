#ifndef TEAM35_CODE35_SRC_SPA_SRC_COMMONS_ASTNODE_H_
#define TEAM35_CODE35_SRC_SPA_SRC_COMMONS_ASTNODE_H_

#include <string>
#include <vector>
#include <optional>
#include <memory>

class ASTNode {
 public:
    enum class SyntaxType {
        Program,
        Procedure,
        StmtLst,
        Variable,
        Constant,

        Assign,
        Read,
        Print,
        If,
        While,
        Else,
        Then,
        Call,

        // logical operators
        LogicalAnd,
        LogicalOr,
        LogicalNot,

        // relational operators
        Equivalence,
        NotEqual,
        LessThan,
        LessThanEqualTo,
        GreaterThan,
        GreaterThanEqualTo,

        // arithmetic operators
        Plus,
        Minus,
        Divide,
        Multiply,
        Modulo,
    };

    ASTNode(SyntaxType syntaxType, std::optional<std::string> label);

    [[nodiscard]] SyntaxType getSyntaxType() const;

    [[nodiscard]] std::string getLabel() const;

    [[nodiscard]] const std::vector<std::shared_ptr<ASTNode>> &getChildren() const;

    void addChild(std::shared_ptr<ASTNode>);

    virtual ~ASTNode() = default;

 private:
    std::optional<std::string> label_;
    SyntaxType type_;
    std::vector<std::shared_ptr<ASTNode> > children_;
};

#endif  // TEAM35_CODE35_SRC_SPA_SRC_COMMONS_ASTNODE_H_
