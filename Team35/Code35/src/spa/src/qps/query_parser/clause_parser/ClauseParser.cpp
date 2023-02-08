#include "ClauseParser.h"

#include <string>
#include <vector>

#include "commons/token/Token.h"


std::vector<Clause *> ClauseParser::parse(TokenValidator &tokenValidator, std::vector<Synonym> synonyms) {
    std::vector<Clause *> result;
    while (1) {
        std::unique_ptr<Token> nextToken = tokenValidator.getNextToken();;
        if (nextToken->getLexeme() == "such") {
            tokenValidator.getNextToken();
            SuchThatClauseParser stcp;
            result.push_back(stcp.parse(tokenValidator, synonyms));
        } else if (nextToken->getLexeme() == "pattern") {
            PatternClauseParser pcp;
            result.push_back(pcp.parse(tokenValidator, synonyms));
        } else if (nextToken->getTag() == Token::Tag::EndOfFile) {
            break;
        }
    }
    return result;
}
