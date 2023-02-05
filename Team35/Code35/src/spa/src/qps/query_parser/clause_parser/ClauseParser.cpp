#include <string>
#include "ClauseParser.h"

std::vector<Clause> ClauseParser::parse(std::unique_ptr<Lexer> lexer, std::vector<Synonym> synonyms) {
    std::vector<Clause> result;
    while (1) {
        Token nextToken = lexer->Scan();
        if (nextToken.GetLexeme() == "such") {
            lexer->Scan();
            result.push_back(suchThatClauseParser.parse(std::move(lexer), synonyms));
        } else if (nextToken.GetLexeme() == "pattern") {
            result.push_back(patternClauseParser.parse(std::move(lexer), synonyms));
        } else if (nextToken.GetTag() == Token::Tag::EndOfFile) {
            break;
        }
    }
    return result;
}
