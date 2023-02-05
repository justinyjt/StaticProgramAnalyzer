#pragma once

#include <memory>
#include <vector>
#include "qps/entities/Synonym.h"
#include "commons/lexer/Lexer.h"

class SelectionParser {
 public:
    Synonym parse(const std::unique_ptr<Lexer> &lexer, std::vector<Synonym> synonyms);
};

