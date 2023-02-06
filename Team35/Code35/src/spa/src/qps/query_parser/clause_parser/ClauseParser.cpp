#include <string>
#include <vector>
#include "ClauseParser.h"

std::vector<Clause*> ClauseParser::parse(const std::unique_ptr<Lexer> &lexer, std::vector<Synonym> synonyms) {
    std::vector<Clause*> result;
    while (1) {
        Token nextToken = lexer->scan();
        if (nextToken.getLexeme() == "such") {
            lexer->scan();
            result.push_back(suchThatClauseParser.parse(std::move(lexer), synonyms));
        } else if (nextToken.getLexeme() == "pattern") {
            result.push_back(patternClauseParser.parse(std::move(lexer), synonyms));
        } else if (nextToken.getTag() == Token::Tag::EndOfFile) {
            break;
        }
    }
    return result;
}
