#include <vector>

#include "ClauseParser.h"
#include "commons/token/Token.h"
#include "qps/query_exceptions/SyntaxException.h"

ClauseParser::ClauseParser(PQLTokenScanner& pqlTokenScanner,
                           std::unordered_map<std::string, Synonym::DesignEntity>& synonyms) :
        pqlTokenScanner(pqlTokenScanner), synonyms(synonyms), suchThatClauseParser(pqlTokenScanner, synonyms),
        patternClauseParser(pqlTokenScanner, synonyms) {}

std::vector<std::unique_ptr<Clause>> ClauseParser::parse() {
    std::vector<std::unique_ptr<Clause>> result;
    while (true) {
        if (pqlTokenScanner.peek(Token::Tag::EndOfFile)) {
            break;
        } else if (pqlTokenScanner.peek(Token::Tag::Such)) {
            result.push_back(suchThatClauseParser.parse());
        } else if (pqlTokenScanner.peek(Token::Tag::Pattern)) {
            result.push_back(patternClauseParser.parse());
        } else {
            throw SyntaxException();
        }
    }
    return result;
}
