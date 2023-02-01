#pragma once

#include <string>
#include <list>
#include "commons/ASTNode.h"

class IParser {
public:
    typedef std::string PROGRAM;
    virtual std::shared_ptr<ASTNode> Parse(PROGRAM src) = 0;
    virtual ~IParser() = default;
};
