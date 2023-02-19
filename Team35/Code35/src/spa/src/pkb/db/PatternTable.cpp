#include <memory>

#include "PatternTable.h"
#include "commons/lexer/Lexer.h"
#include "commons/lexer/LexerFactory.h"

bool PatternTable::addPattern(STMT_NUM stmt, std::string pattern) {
    stmtPatternMap.insert({stmt, pattern});
    return true;
}

STMT_SET PatternTable::getExactPatternMatch(std::string pattern) const {
    STMT_SET result;

    for (auto storedPattern : stmtPatternMap) {
        if (storedPattern.second == pattern) {
            result.emplace(storedPattern.first);
        }
    }
    return result;
}

STMT_SET PatternTable::getPartialPatternMatch(std::string pattern) const {
    STMT_SET  result;
    // TODO fix temp solution
    // this assumes pattern is one token
    if (pattern.empty()) {
        return result;
    }
    std::unique_ptr<ILexer> lexer = LexerFactory::createLexer(pattern, LexerFactory::LexerType::Expression);
    std::unique_ptr<Token> token;
    std::unique_ptr<Token> patternToken = lexer->scan();
    for (auto storedPattern : stmtPatternMap) {
        lexer = LexerFactory::createLexer(storedPattern.second, LexerFactory::LexerType::Expression);
        token = lexer->scan();
        while (token->getTag() != Token::Tag::EndOfFile) {
            if (token->getLexeme() == patternToken->getLexeme()) {
                result.emplace(storedPattern.first);
                break;
            }
            token = lexer->scan();
        }
    }

    return result;
}
