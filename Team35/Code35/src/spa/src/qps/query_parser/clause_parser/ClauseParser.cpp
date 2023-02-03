#include <string>
#include "ClauseParser.h"

std::vector<Clause> ClauseParser::parse(Lexer &lexer, std::vector<std::string> synonyms) {
    std::vector<Clause> result;
    // parse relationships and pattern in a loop
    while(1) {
        Token nextToken = lexer.Scan();
        if (nextToken.GetStringValue() == "such") {
            lexer.Scan();
            result.push_back(suchThatClauseParser.parse(&lexer, synonyms));
        } else if (nextToken.GetStringValue() == "pattern") {
            result.push_back(patternClauseParser.parse(&lexer, synonyms));
        }
    }
    return result;
}
