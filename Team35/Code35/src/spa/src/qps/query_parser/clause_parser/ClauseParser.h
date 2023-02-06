#pragma once

#include <memory>
#include <vector>
#include <utility>
#include "qps/query_parser/clause_parser/such_that_clause_parser/SuchThatClauseParser.h"
#include "qps/query_parser/clause_parser/pattern_clause_parser/PatternClauseParser.h"
#include "qps/pql/Synonym.h"
#include "commons/lexer/Lexer.h"

class ClauseParser {
 public:
    std::vector<Clause*> parse(const std::unique_ptr<Lexer> &lexer, std::vector<Synonym> synonyms);
 private:
    PatternClauseParser patternClauseParser;
    SuchThatClauseParser suchThatClauseParser;
};
