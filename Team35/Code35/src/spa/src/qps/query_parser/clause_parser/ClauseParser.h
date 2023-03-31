#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

#include "qps/clause/OptimisableClause.h"
#include "qps/query_parser/clause_parser/such_that_clause_parser/SuchThatClauseParser.h"
#include "qps/query_parser/clause_parser/pattern_clause_parser/PatternClauseParser.h"
#include "qps/pql/Synonym.h"
#include "commons/lexer/Lexer.h"
#include "commons/token_scanner/TokenScanner.h"
#include "qps/query_parser/clause_parser/with_clause_parser/WithClauseParser.h"

class ClauseParser {
 public:
    ClauseParser(PQLTokenScanner &pqlTokenScanner, std::unordered_map<std::string, Synonym::DesignEntity> &synonyms);
    std::vector<std::unique_ptr<OptimisableClause>> parse();
 private:
    PatternClauseParser patternClauseParser;
    SuchThatClauseParser suchThatClauseParser;
    WithClauseParser withClauseParser;
    PQLTokenScanner &pqlTokenScanner;
    std::unordered_map<std::string, Synonym::DesignEntity> &synonyms;
};
