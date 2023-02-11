#pragma once

#include <memory>
#include <string>
#include <vector>

#include "qps/pql/Synonym.h"
#include "qps/query_parser/clause_parser/TokenValidator.h"

class DeclarationParser {
 public:
    std::vector<Synonym> parse(TokenValidator &tokenValidator);
    Synonym::DesignEntity processDesignEntity(std::unique_ptr<Token> token);
    bool isDeclared(std::string value, std::vector<Synonym> declarationList);
};
