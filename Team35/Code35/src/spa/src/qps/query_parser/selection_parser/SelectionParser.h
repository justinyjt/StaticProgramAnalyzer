#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "qps/pql/Synonym.h"
#include "commons/lexer/Lexer.h"
#include "qps/clause/Clause.h"
#include "qps/clause/select_clause/SelectClause.h"
#include "commons/token_scanner/TokenScanner.h"
#include "commons/token_scanner/PQLTokenScanner.h"

class SelectionParser {
 public:
    explicit SelectionParser(PQLTokenScanner &pqlTokenScanner,
                             std::unordered_map<std::string, Synonym::DesignEntity>& synonyms);
    std::unique_ptr<SelectClause> parse();
    std::unique_ptr<SelectClause> parseSelect();
 private:
    PQLTokenScanner &pqlTokenScanner;
    std::unordered_map<std::string, Synonym::DesignEntity>& synonyms;
};
