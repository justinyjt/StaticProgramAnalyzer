#pragma once

#include <memory>
#include <vector>
#include "qps/pql/Synonym.h"
#include "commons/lexer/Lexer.h"
#include "qps/query_parser/clause_parser/TokenValidator.h"

class SelectionParser {
 public:
    Synonym parse(TokenValidator &tokenValidator, std::vector<Synonym> synonyms);
};
