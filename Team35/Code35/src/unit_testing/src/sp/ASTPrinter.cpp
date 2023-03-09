#include "ASTPrinter.h"

ASTPrinter::ASTPrinter() : res_() {}

std::string ASTPrinter::printAST(const std::shared_ptr<ASTNode> &node) {
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
        }
    } else if (node->getSyntaxType() == ASTNode::SyntaxType::Assign) {
        const auto &lAssign = node->getChildren().front();
        const auto &rAssign = node->getChildren().back();
        res.append(printAST(lAssign));
        res.append("=");
        res.append(printAST(rAssign));
        res.append(";\n");
    } else if (node->getSyntaxType() == ASTNode::SyntaxType::Read) {
        const auto &child = node->getChildren().front();
        res.append("read " + printAST(child));
        res.append(";\n");
    } else if (node->getSyntaxType() == ASTNode::SyntaxType::Print) {
        const auto &child = node->getChildren().front();
        res.append("print " + printAST(child));
        res.append(";\n");
    } else if (node->getSyntaxType() == ASTNode::SyntaxType::If) {
        const auto &cond = node->getChildren().at(0);
        const auto &thenStmt = node->getChildren().at(1);
        const auto &elseStmt = node->getChildren().at(2);
        res.append("if (" + printAST(cond) + ") then {\n" + printAST(thenStmt) + "} else {\n" + printAST(elseStmt) +
                   "}\n");
    } else if (node->getSyntaxType() == ASTNode::SyntaxType::While) {
        const auto &cond = node->getChildren().at(0);
        const auto &thenStmt = node->getChildren().at(1);
        res.append("while (" + printAST(cond) + ") {\n" + printAST(thenStmt) + "}\n");
    } else if (node->getSyntaxType() == ASTNode::SyntaxType::Variable
               || node->getSyntaxType() == ASTNode::SyntaxType::Constant) {
        return node->getLabel();
    } else { //binary operations
        if (node->getLabel() == "!") {
            res.append(node->getLabel());
            const auto &operand1 = node->getChildren().front();
            res.append("(" + printAST(operand1) + ")");
        } else if (node->getLabel() == "&&" || node->getLabel() == "||") {
            const auto &operand1 = node->getChildren().front();
            const auto &operand2 = node->getChildren().back();
            res.append("(" + printAST(operand1) + ")");
            res.append(node->getLabel());
            res.append("(" + printAST(operand2) + ")");
        } else {
            const auto &operand1 = node->getChildren().front();
            const auto &operand2 = node->getChildren().back();
            res.append(printAST(operand1));
            res.append(node->getLabel());
            res.append(printAST(operand2));
        }
    }
    return res;
}
