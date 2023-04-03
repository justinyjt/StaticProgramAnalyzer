#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "qps/pql/Synonym.h"
#include "commons/token_scanner/PQLTokenScanner.h"

class DeclarationParser {
 public:
    explicit DeclarationParser(PQLTokenScanner &pqlTokenScanner,
                               std::unordered_map<std::string, Synonym::DesignEntity>& synonyms);
    std::unordered_map<std::string, Synonym::DesignEntity> parse();
    Synonym::DesignEntity parseDesignEntity();
    std::string parseSynonym();
    void parseDeclarations(Synonym::DesignEntity de);
 private:
    PQLTokenScanner& pqlTokenScanner;
    std::unordered_map<std::string, Synonym::DesignEntity>& synonyms;
};
