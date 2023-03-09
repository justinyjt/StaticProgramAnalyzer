#pragma once

#include <memory>
#include <string>
#include <vector>

#include "qps/pql/Synonym.h"
#include "qps/clause/Clause.h"
#include "commons/token/Token.h"
#include "commons/lexer/Lexer.h"
#include "qps/query_parser/clause_parser/TokenValidator.h"
#include "commons/token_scanner/PQLTokenScanner.h"
#include "commons/SynonymHash.h"

class SuchThatClauseParser {
 public:
    explicit SuchThatClauseParser(PQLTokenScanner& pqlTokenScanner, std::unordered_map<std::string, Synonym::DesignEntity>& synonyms);
    std::unique_ptr<Clause> parse();
    std::unique_ptr<Clause> parseRelationship();
    std::unique_ptr<Clause> parseUsesModifies(std::string& relationship);
    std::unique_ptr<Clause> parseStmtStmt(std::string& relationship);
    std::unique_ptr<Clause> parseEntEnt(std::string& relationship);
    std::unique_ptr<PQLToken> parseEntRef();
    std::unique_ptr<PQLToken> parseStmtRef();
    std::unique_ptr<Clause> createClause(std::unique_ptr<PQLToken> token1, std::unique_ptr<PQLToken> token2,
                         const std::string& relationship);
    bool isStmtRef();
    bool isEntRef();
    bool isName(std::string input);
private:
    PQLTokenScanner &pqlTokenScanner;
    std::unordered_map<std::string, Synonym::DesignEntity>& synonyms;
};
