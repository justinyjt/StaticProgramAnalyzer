#pragma once

#include <string>
#include "commons/ASTNode.h"

class ASTPrinter {
public:
    ASTPrinter();
    std::string printAST(const std::unique_ptr<ASTNode>&);

private:
    std::string res_;
};
