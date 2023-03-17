#include <vector>

#include "ClauseParser.h"
#include "commons/token/Token.h"
#include "qps/query_exceptions/SyntaxException.h"

ClauseParser::ClauseParser(PQLTokenScanner& pqlTokenScanner,
                           std::unordered_map<std::string, Synonym::DesignEntity>& synonyms) :
        pqlTokenScanner(pqlTokenScanner), synonyms(synonyms), suchThatClauseParser(pqlTokenScanner, synonyms),
        patternClauseParser(pqlTokenScanner, synonyms), withClauseParser(pqlTokenScanner, synonyms) {}

std::vector<std::unique_ptr<Clause>> ClauseParser::parse() {
    std::vector<std::unique_ptr<Clause>> result;
    while (true) {
        if (pqlTokenScanner.peek(Token::Tag::EndOfFile)) {
            break;
        } else if (pqlTokenScanner.peek(Token::Tag::Such)) {
            std::vector<std::unique_ptr<Clause>> clauses = suchThatClauseParser.parse();
            for (std::unique_ptr<Clause> &clause : clauses) {
                result.push_back(std::move(clause));
            }
        } else if (pqlTokenScanner.peek(Token::Tag::Pattern)) {
            std::vector<std::unique_ptr<Clause>> clauses = patternClauseParser.parse();
            for (std::unique_ptr<Clause> &clause : clauses) {
                result.push_back(std::move(clause));
            }
        } else if (pqlTokenScanner.peek(Token::Tag::With)) {
            std::vector<std::unique_ptr<Clause>> clauses = withClauseParser.parse();
            for (std::unique_ptr<Clause> &clause : clauses) {
                result.push_back(std::move(clause));
            }
        } else {
            throw SyntaxException();
        }
    }
    return result;
}
