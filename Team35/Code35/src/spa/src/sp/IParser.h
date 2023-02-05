#pragma once

#include <string>
#include <memory>
#include "commons/ASTNode.h"
#include "Lexer.h"

class IParser {
 public:
    typedef std::string PROGRAM;
    virtual std::unique_ptr<ASTNode> Parse() = 0;
    virtual ~IParser() = default;
};
