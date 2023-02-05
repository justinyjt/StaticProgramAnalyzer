#pragma once

#include <memory>
#include <vector>
#include "qps/entities/Synonym.h"
#include "commons/lexer/Lexer.h"

class DeclarationParser {
 public:
    std::vector<Synonym> parse(const std::unique_ptr<Lexer> &lexer);
};

