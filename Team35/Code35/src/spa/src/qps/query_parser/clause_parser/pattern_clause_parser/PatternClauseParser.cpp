#include <string>
#include "PatternClauseParser.h"
#include "qps/clauses/patterns/pattern_clauses/AssignPattern.h"

Pattern PatternClauseParser::parse(Lexer &lexer, std::vector<std::string> synonyms) {
    std::string left;
    std::string right;
    std::string pattern_type = lexer.Scan().GetLexeme();

    if (lexer.Scan().GetTag() != Tag::LParen) {
        throw std::runtime_error("missing left parenthesis");
    }

    Token left_arg = lexer.Scan().GetTag();

    if (left_arg != Tag::String) {
        throw std::runtime_error("string expected");
    } else {
        left = left_arg.GetLexeme();
    }

    if (lexer.Scan().GetTag() != Tag::Comma) {
        throw std::runtime_error("missing comma");
    }

    Token right_arg = lexer.Scan().GetTag();

    if (right_arg != Tag::String) {
        throw std::runtime_error("string expected");
    } else {
        right = left_arg.GetLexeme();
    }

    if (lexer.Scan().GetTag() != Tag::RParen) {
        throw std::runtime_error("missing right parenthesis");
    }

    AssignPattern a(left, right);
    return a;
}