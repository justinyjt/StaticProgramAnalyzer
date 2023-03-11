#pragma once

#include <memory>
#include <string>

#include "commons/ASTNode.h"

class ASTPrinter {
 public:
    ASTPrinter();

    std::string printAST(const std::shared_ptr<ASTNode> &);

 private:
    std::string res_;
};
