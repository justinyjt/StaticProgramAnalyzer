#include "ClauseParser.h"
#include <vector>

#include "commons/token/Token.h"
#include "qps/query_exceptions/SyntaxException.h"


std::vector<std::unique_ptr<Clause>> ClauseParser::parse(TokenValidator &tokenValidator,
                                                         std::vector<Synonym>& synonyms) {
    std::vector<std::unique_ptr<Clause>> result;
    while (true) {
        if (tokenValidator.isNextTokenType(Token::Tag::EndOfFile)) {
            break;
        } else if (tokenValidator.isNextTokenType(Token::Tag::Such)) {
            result.push_back(suchThatClauseParser->parse(tokenValidator, synonyms));
        } else if (tokenValidator.isNextTokenType(Token::Tag::Pattern)) {
            result.push_back(patternClauseParser->parse(tokenValidator, synonyms));
        } else {
            throw SyntaxException();
        }
    }
    return result;
}
