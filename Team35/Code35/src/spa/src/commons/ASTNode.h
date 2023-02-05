#ifndef SPA_ASTNODE_H
#define SPA_ASTNODE_H

#include <string>
#include <vector>
#include <optional>

class ASTNode {

public:
    enum class SyntaxType {
        program,
        procedure,
        stmtLst,
        assign,
        var,
        constVal,
        plus,
        minus,
        times,
        div,
        mod,
    };

    ASTNode(SyntaxType syntaxType, std::optional<std::string> label);

    [[nodiscard]] SyntaxType getSyntaxType() const;
    [[nodiscard]] std::string getLabel() const;
    [[nodiscard]] const std::vector<std::unique_ptr<ASTNode>> & getChildren() const;

    void addChild(std::unique_ptr<ASTNode>);

private:
    std::optional<std::string> label_;
    SyntaxType type_;
    std::vector< std::unique_ptr<ASTNode> > children_;
};

#endif //SPA_ASTNODE_H
