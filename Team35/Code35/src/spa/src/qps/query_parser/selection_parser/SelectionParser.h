#pragma once

#include <memory>
#include <string>
#include <vector>

#include "qps/pql/Synonym.h"
#include "commons/lexer/Lexer.h"
#include "qps/query_parser/clause_parser/TokenValidator.h"
#include "qps/clause/Clause.h"
#include "qps/clause/SelectClause.h"
#include "commons/token_scanner/TokenScanner.h"
#include "commons/token_scanner/PQLTokenScanner.h"

class SelectionParser {
 public:
    explicit SelectionParser(PQLTokenScanner &pqlTokenScanner, std::vector<Synonym>& synonyms);
    std::unique_ptr<SelectClause> parse();
    std::unique_ptr<SelectClause> parseSelect();
    bool isSynonymDeclared(std::string selectedSynonym);
    bool isName(std::string input);
private:
    PQLTokenScanner &pqlTokenScanner;
    std::vector<Synonym>& synonyms;
};
