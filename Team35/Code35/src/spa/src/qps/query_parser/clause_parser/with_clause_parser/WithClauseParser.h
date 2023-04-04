#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "qps/pql/Synonym.h"
#include "qps/clause/OptimisableClause.h"
#include "commons/token/Token.h"
#include "commons/lexer/Lexer.h"
#include "commons/token_scanner/PQLTokenScanner.h"

class WithClauseParser {
 public:
    explicit WithClauseParser(PQLTokenScanner &pqlTokenScanner,
                              std::unordered_map<std::string, Synonym::DesignEntity> &synonyms);
    std::vector<std::unique_ptr<OptimisableClause>> parse();
    std::unique_ptr<OptimisableClause> parseWith();
    std::unique_ptr<PQLToken> parseRef();
    std::unique_ptr<PQLToken> parseAttrRef();
    std::unique_ptr<PQLToken> parseAttrName(Synonym::DesignEntity de, std::string &synonym);
    std::unique_ptr<PQLToken> parseProcNameAttrName(Synonym::DesignEntity de, std::string &synonym);
    std::unique_ptr<PQLToken> parseVarNameAttrName(Synonym::DesignEntity de, std::string &synonym);
    std::unique_ptr<PQLToken> parseValueAttrName(Synonym::DesignEntity de, std::string &synonym);
    std::unique_ptr<PQLToken> parseStmtNumAttrName(Synonym::DesignEntity de, std::string &synonym);
 private:
    PQLTokenScanner &pqlTokenScanner;
    std::unordered_map<std::string, Synonym::DesignEntity> &synonyms;
    int entArgCount = 0;
    int numArgCount = 0;
};
