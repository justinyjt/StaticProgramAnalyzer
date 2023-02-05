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
    static std::unique_ptr<ILexer> createLexer(Source source, LexerType lexer_type);

 private:
    static std::unique_ptr<ILexer> createSimpleLexer(Source source);
    static std::unique_ptr<ILexer> createPqlLexer(Source source);
    static std::unique_ptr<ILexer> createExpressionLexer(Source source);
};
