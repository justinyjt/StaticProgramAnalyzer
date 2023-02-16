#pragma once

#include <string>
#include <memory>
#include "commons/ASTNode.h"
#include "commons/lexer/Lexer.h"
#include "commons/lexer/LexerFactory.h"

class IParser {
 public:
    virtual std::unique_ptr<ASTNode> Parse() = 0;
    virtual ~IParser() = default;
};
