#pragma once

#include <memory>
#include <string>
#include <vector>

#include "qps/pql/Synonym.h"
#include "qps/query_parser/clause_parser/TokenValidator.h"
#include "commons/token_scanner/PQLTokenScanner.h"

class DeclarationParser {
 public:
    explicit DeclarationParser(PQLTokenScanner &pqlTokenScanner, std::vector<Synonym>& synonyms);
    std::vector<Synonym> parse();
    Synonym::DesignEntity parseDesignEntity();
    std::unique_ptr<Synonym> parseSynonym(Synonym::DesignEntity);
    bool isValidName(std::string input);
    void validateAndAddSynonym(std::unique_ptr<Synonym> synonym);
private:
    PQLTokenScanner& pqlTokenScanner;
    std::vector<Synonym>& synonyms;
};
