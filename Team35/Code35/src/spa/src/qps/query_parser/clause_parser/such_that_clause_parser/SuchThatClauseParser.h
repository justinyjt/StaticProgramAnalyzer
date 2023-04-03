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

class SuchThatClauseParser {
 public:
    explicit SuchThatClauseParser(PQLTokenScanner &pqlTokenScanner,
                                  std::unordered_map<std::string, Synonym::DesignEntity> &synonyms);
    std::vector<std::unique_ptr<OptimisableClause>> parse();
    std::unique_ptr<OptimisableClause> parseRelationship();
    std::unique_ptr<OptimisableClause> parseUsesModifies(std::string &relationship);
    std::unique_ptr<OptimisableClause> parseStmtStmt(std::string &relationship);
    std::unique_ptr<OptimisableClause> parseEntEnt(std::string &relationship);
    std::unique_ptr<PQLToken> parseEntRef();
    std::unique_ptr<PQLToken> parseStmtRef();
 private:
    PQLTokenScanner &pqlTokenScanner;
    std::unordered_map<std::string, Synonym::DesignEntity> &synonyms;
};
