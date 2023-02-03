#include "SuchThatClauseParser.h"
#include "qps/clauses/relationships/relationship_clauses/Modify.h"

Relationship SuchThatClauseParser::parse(Lexer &lexer, std::vector<std::string> synonyms) {
    int left;
    std::string right;

    std::string keyword = lexer.Scan().GetLexeme();

    if (lexer.Scan().GetTag() != Tag::LParen) {
        throw std::runtime_error("missing left parenthesis");
    }

    Token left_arg = lexer.Scan().GetTag();

    if (left_arg != Tag::Integer) {
        throw std::runtime_error("integer expected");
    } else {
        left = left_arg.GetInteger();
    }

    if (lexer.Scan().GetTag() != Tag::Comma) {
        throw std::runtime_error("missing comma");
    }

    Token right_arg = lexer.Scan().GetTag();

    if (right_arg != Tag::String) {
        throw std::runtime_error("string expected");
    } else {
        if (std::count(synonyms.begin(), synonyms.end(), right_arg.GetLexeme())) {
            return right_arg.GetLexeme();
        } else {
            throw std::runtime_error("synonym not declared!");
        }
        right = right_arg.GetLexeme();
    }

    if (lexer.Scan().GetTag() != Tag::RParen) {
        throw std::runtime_error("missing right parenthesis");
    }

    if (keyword == "modifies") {
        Modify m(left, right);
        return m;
    } else {
        throw std::runtime_error("not modifies clause");
    }
}
