#pragma once

#include <memory>
#include <vector>

#include "qps/pql/Synonym.h"
#include "qps/query_parser/clause_parser/TokenValidator.h"

class DeclarationParser {
 public:
    std::vector<Synonym> parse(TokenValidator &tokenValidator);
};
