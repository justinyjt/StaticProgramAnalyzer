#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <utility>

#include "qps/clause/OptimisableClause.h"
#include "qps/clause/two_arg_clause/AssignPatternClause.h"
#include "qps/pql/Synonym.h"
#include "qps/pql/Wildcard.h"
#include "commons/token/Token.h"
#include "commons/lexer/Lexer.h"
#include "commons/token_scanner/TokenScanner.h"
#include "commons/token_scanner/PQLTokenScanner.h"

class PatternClauseParser {
 public:
    PatternClauseParser(PQLTokenScanner &pqlTokenScanner,
                        std::unordered_map<std::string, Synonym::DesignEntity> &synonyms);

    std::vector<std::unique_ptr<OptimisableClause>> parse();

    std::unique_ptr<OptimisableClause> parsePattern();

    std::unique_ptr<OptimisableClause> parseAssign(const std::string &patternSynonym);

    std::unique_ptr<OptimisableClause> parseWhile(const std::string &patternSynonym);

    std::unique_ptr<OptimisableClause> parseIf(const std::string &patternSynonym);

    std::unique_ptr<PQLToken> parseVar();

    std::unique_ptr<PQLToken> parseEntRef();

    std::unique_ptr<PQLToken> parseExpressionSpec();

    Synonym::DesignEntity parsePatternSynonym();

 private:
    PQLTokenScanner &pqlTokenScanner;
    std::unordered_map<std::string, Synonym::DesignEntity> &synonyms;
};
