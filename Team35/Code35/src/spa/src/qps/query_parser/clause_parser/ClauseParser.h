#pragma once

#include <memory>
#include <vector>
#include <utility>

#include "qps/query_parser/clause_parser/such_that_clause_parser/SuchThatClauseParser.h"
#include "qps/query_parser/clause_parser/pattern_clause_parser/PatternClauseParser.h"
#include "qps/pql/Synonym.h"
#include "commons/lexer/Lexer.h"
#include "TokenValidator.h"
#include "commons/token_scanner/TokenScanner.h"

class ClauseParser {
 public:
    ClauseParser(PQLTokenScanner &pqlTokenScanner, std::vector<Synonym> &synonyms);
    std::vector<std::unique_ptr<Clause>> parse();
 private:
    PatternClauseParser patternClauseParser;
    SuchThatClauseParser suchThatClauseParser;
    PQLTokenScanner& pqlTokenScanner;
    std::vector<Synonym>& synonyms;
};
