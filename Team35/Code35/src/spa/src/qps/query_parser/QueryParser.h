#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <string>
#include "qps/clauses/Clause.h"
#include "qps/entities/Synonym.h"

class QueryParser {
 public:
    void setLexer(std::unique_ptr<Lexer> lexer);
    std::pair<Synonym, std::vector<Clause>> parse();
 private:
    std::unique_ptr<Lexer> lexer_;
};
