#include "ClauseParser.h"

#include <string>
#include <vector>

#include "commons/token/Token.h"

std::vector<Clause *> ClauseParser::parse(const std::unique_ptr<ILexer> &lexer, std::vector<Synonym> synonyms) {
    std::vector<Clause *> result;
    while (1) {
        std::unique_ptr<Token> nextToken = lexer->scan();
        if (nextToken->getLexeme() == "such") {
            lexer->scan();
            result.push_back(suchThatClauseParser.parse(std::move(lexer), synonyms));
        } else if (nextToken->getLexeme() == "pattern") {
            result.push_back(patternClauseParser.parse(std::move(lexer), synonyms));
        } else if (nextToken->getTag() == Token::Tag::EndOfFile) {
            break;
        }
    }
    return result;
}
