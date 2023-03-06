#pragma once

#include <memory>
#include <string>
#include <vector>

#include "qps/pql/Synonym.h"
#include "commons/lexer/Lexer.h"
#include "qps/query_parser/clause_parser/TokenValidator.h"
#include "qps/clause/Clause.h"
#include "qps/clause/SelectClause.h"

class SelectionParser {
 public:
    std::unique_ptr<SelectClause> parse(TokenValidator& tokenValidator, std::vector<Synonym>& synonyms);
    bool isSynonymDeclared(std::string selectedSynonym, std::vector<Synonym>& synonyms);
};
