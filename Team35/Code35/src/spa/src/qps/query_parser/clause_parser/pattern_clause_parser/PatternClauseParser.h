#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <utility>

#include "qps/clause/Clause.h"
#include "qps/clause/two_arg_clause/AssignPattern.h"
#include "qps/pql/Synonym.h"
#include "qps/pql/Wildcard.h"
#include "commons/token/Token.h"
#include "commons/lexer/Lexer.h"
#include "commons/token_scanner/TokenScanner.h"
#include "commons/token_scanner/PQLTokenScanner.h"

class PatternClauseParser {
 public:
    PatternClauseParser(PQLTokenScanner& pqlTokenScanner,
                        std::unordered_map<std::string, Synonym::DesignEntity>& synonyms);
    std::unique_ptr<Clause> parse();
    std::unique_ptr<Clause> parsePattern();
    std::unique_ptr<Clause> parseAssign(std::string patternSynonym);
    std::unique_ptr<Clause> parseWhile(std::string patternSynonym);
    std::unique_ptr<Clause> parseIf(std::string patternSynonym);
    std::unique_ptr<PQLToken> parseEntRef();
    std::unique_ptr<PQLToken> parseExpressionSpec();
    std::unique_ptr<Clause> createClause(std::unique_ptr<PQLToken> token1, std::unique_ptr<PQLToken> token2,
                                            const std::string& patternStr);
    Synonym::DesignEntity parsePatternSynonym();
    bool isValidExpr();
 private:
    PQLTokenScanner& pqlTokenScanner;
    std::unordered_map<std::string, Synonym::DesignEntity>& synonyms;
};
