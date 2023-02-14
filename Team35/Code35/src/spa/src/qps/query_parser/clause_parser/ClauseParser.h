#pragma once

#include <memory>
#include <vector>
#include <utility>

#include "qps/query_parser/clause_parser/such_that_clause_parser/SuchThatClauseParser.h"
#include "qps/query_parser/clause_parser/pattern_clause_parser/PatternClauseParser.h"
#include "qps/pql/Synonym.h"
#include "commons/lexer/Lexer.h"
#include "TokenValidator.h"

class ClauseParser {
 public:
    std::vector<Clause *> parse(TokenValidator &tokenValidator, std::vector<Synonym> synonyms);
 private:
    PatternClauseParser patternClauseParser;
    SuchThatClauseParser suchThatClauseParser;
};
