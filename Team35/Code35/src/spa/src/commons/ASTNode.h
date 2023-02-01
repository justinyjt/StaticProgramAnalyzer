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

    void addChild(const std::shared_ptr<ASTNode> &newChild);

private:
    std::optional<std::string> label;
    SyntaxType type;
    std::vector< std::shared_ptr<ASTNode> > children;
};

#endif //SPA_ASTNODE_H
