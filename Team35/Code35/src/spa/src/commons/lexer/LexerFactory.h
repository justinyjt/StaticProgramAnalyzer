#pragma once

#include <memory>

#include "ILexer.h"
#include "Lexer.h"

class LexerFactory {
 public:
    enum class LexerType {
        Simple,
        Pql,
        Expression,
    };
    static std::unique_ptr<ILexer> CreateLexer(Source source, LexerType lexer_type);

 private:
    static std::unique_ptr<ILexer> CreateSimpleLexer(Source source);
    static std::unique_ptr<ILexer> CreatePqlLexer(Source source);
    static std::unique_ptr<ILexer> CreateExpressionLexer(Source source);
};
