#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "qps/pql/Synonym.h"
#include "qps/clause/Clause.h"
#include "commons/token/Token.h"
#include "commons/lexer/Lexer.h"
#include "commons/token_scanner/PQLTokenScanner.h"

class WithClauseParser {
 public:
    explicit WithClauseParser(PQLTokenScanner& pqlTokenScanner,
                                  std::unordered_map<std::string, Synonym::DesignEntity>& synonyms);
    std::vector<std::unique_ptr<Clause>> parse();
    std::unique_ptr<Clause> parseWith();
    std::unique_ptr<PQLToken> parseRef();
 private:
    PQLTokenScanner &pqlTokenScanner;
    std::unordered_map<std::string, Synonym::DesignEntity>& synonyms;
    int entArgCount = 0;
    int numArgCount = 0;
};