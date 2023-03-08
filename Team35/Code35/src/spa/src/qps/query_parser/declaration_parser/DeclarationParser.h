#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "qps/pql/Synonym.h"
#include "qps/query_parser/clause_parser/TokenValidator.h"
#include "commons/token_scanner/PQLTokenScanner.h"
#include "commons/SynonymHash.h"

class DeclarationParser {
 public:
    explicit DeclarationParser(PQLTokenScanner &pqlTokenScanner, std::unordered_map<std::string, Synonym::DesignEntity>& synonyms);
    std::unordered_map<std::string, Synonym::DesignEntity> parse();
    Synonym::DesignEntity parseDesignEntity();
    std::string parseSynonym(Synonym::DesignEntity);
    bool isName(std::string input);
private:
    PQLTokenScanner& pqlTokenScanner;
    std::unordered_map<std::string, Synonym::DesignEntity>& synonyms;
};
