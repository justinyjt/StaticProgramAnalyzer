#pragma once

#include <memory>
#include <vector>
#include "qps/entities/Synonym.h"

class DeclarationParser {
 public:
    std::vector<Synonym> parse(std::unique_ptr<Lexer> lexer);
};

