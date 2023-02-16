#include "ASTPrinter.h"

ASTPrinter::ASTPrinter() : res_() {}

std::string ASTPrinter::printAST(const std::unique_ptr<ASTNode> &node) {
    std::string res;
    if (node->getSyntaxType() == ASTNode::SyntaxType::Program) {
        for (auto &child : node->getChildren()) {
            res.append(printAST(child));
        }
    } else if (node->getSyntaxType() == ASTNode::SyntaxType::Procedure) {
        res.append("procedure " + node->getLabel() + " {\n");
        for (auto &child : node->getChildren()) {
            res.append(printAST(child));
        }
        res.append("}\n");
    } else if (node->getSyntaxType() == ASTNode::SyntaxType::StmtLst) {
        for (auto &child : node->getChildren()) {
            res.append(printAST(child));
            res.append(";\n");
        }
    } else if (node->getSyntaxType() == ASTNode::SyntaxType::Assign) {
        const auto &lAssign = node->getChildren().front();
        const auto &rAssign = node->getChildren().back();
        res.append(printAST(lAssign));
        res.append("=");
        res.append(printAST(rAssign));
    } else if (node->getSyntaxType() == ASTNode::SyntaxType::Read) {
        const auto &child = node->getChildren().front();
        res.append("read " + printAST(child));
    } else if (node->getSyntaxType() == ASTNode::SyntaxType::Print) {
        const auto &child = node->getChildren().front();
        res.append("print " + printAST(child));
    } else if (node->getSyntaxType() == ASTNode::SyntaxType::Variable
        || node->getSyntaxType() == ASTNode::SyntaxType::Constant) {
        return node->getLabel();
    } else { //binary operations
        const auto &lAssign = node->getChildren().front();
        const auto &rAssign = node->getChildren().back();
        res.append(printAST(lAssign));
        res.append(node->getLabel());
        res.append(printAST(rAssign));
    }
    return res;
}
