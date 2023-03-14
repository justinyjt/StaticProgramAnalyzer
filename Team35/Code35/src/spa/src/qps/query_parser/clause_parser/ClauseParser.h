#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

#include "qps/query_parser/clause_parser/such_that_clause_parser/SuchThatClauseParser.h"
#include "qps/query_parser/clause_parser/pattern_clause_parser/PatternClauseParser.h"
#include "qps/pql/Synonym.h"
#include "commons/lexer/Lexer.h"
#include "commons/token_scanner/TokenScanner.h"

class ClauseParser {
 public:
    ClauseParser(PQLTokenScanner& pqlTokenScanner, std::unordered_map<std::string, Synonym::DesignEntity>& synonyms);
    std::vector<std::unique_ptr<Clause>> parse();
 private:
    PatternClauseParser patternClauseParser;
    SuchThatClauseParser suchThatClauseParser;
    PQLTokenScanner& pqlTokenScanner;
    std::unordered_map<std::string, Synonym::DesignEntity>& synonyms;
};
