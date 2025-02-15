#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "qps/pql/Synonym.h"
#include "commons/lexer/Lexer.h"
#include "qps/clause/Clause.h"
#include "qps/clause/select_clause/SelectClause.h"
#include "commons/token_scanner/TokenScanner.h"
#include "commons/token_scanner/PQLTokenScanner.h"
#include "qps/clause/select_clause/MultipleSynonymSelectClause.h"

class SelectionParser {
 public:
    explicit SelectionParser(PQLTokenScanner &pqlTokenScanner,
                             std::unordered_map<std::string, Synonym::DesignEntity>& synonyms);
    std::unique_ptr<SelectClause> parse();
 private:
    PQLTokenScanner &pqlTokenScanner;
    std::unordered_map<std::string, Synonym::DesignEntity>& synonyms;

    std::vector<std::unique_ptr<Synonym>> parseMultiSelect();
    std::unique_ptr<Synonym> parseSelect();
    std::unique_ptr<Synonym> parseAttrName(Synonym::DesignEntity de, std::string &selected);
    std::unique_ptr<Synonym> parseProcNameAttrName(Synonym::DesignEntity de, std::string &selected);
    std::unique_ptr<Synonym> parseVarNameAttrName(Synonym::DesignEntity de, std::string &selected);
    std::unique_ptr<Synonym> parseValueAttrName(Synonym::DesignEntity de, std::string &selected);
    std::unique_ptr<Synonym> parseStmtNumAttrName(Synonym::DesignEntity de, std::string &selected);
};
