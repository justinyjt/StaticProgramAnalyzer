#pragma once

#include <memory>

#include "commons/ASTNode.h"

class IParser {
 public:
    virtual std::shared_ptr<ASTNode> parse() = 0;

    virtual ~IParser() = default;
};
