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
    static std::unique_ptr<ILexer> createLexer(const Source &source, LexerType lexer_type);

 private:
    static std::unique_ptr<ILexer> createSimpleLexer(const Source &source);
    static std::unique_ptr<ILexer> createPqlLexer(const Source &source);
    static std::unique_ptr<ILexer> createExpressionLexer(const Source &source);
};
