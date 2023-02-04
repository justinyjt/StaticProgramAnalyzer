#include <string>
#include "ClauseParser.h"

std::vector<Clause> ClauseParser::parse(std::unique_ptr<Lexer> lexer, std::vector<Synonym> synonyms) {
    std::vector<Clause> result;
    // parse relationships and pattern in a loop
    while(1) {
        Token nextToken = lexer->Scan();
        if (nextToken.GetStringValue() == "such") {
            lexer->Scan();
            result.push_back(suchThatClauseParser.parse(std::move(lexer), synonyms));
        } else if (nextToken.GetStringValue() == "pattern") {
            result.push_back(patternClauseParser.parse(std::move(lexer), synonyms));
        } else {
            throw std::runtime_error("invalid syntax");
        }
    }
    return result;
}
