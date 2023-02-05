#ifndef TEAM35_CODE35_SRC_SPA_SRC_SP_LEXER_H_
#define TEAM35_CODE35_SRC_SPA_SRC_SP_LEXER_H_
#include <string>
#include "commons/Token.h"


class Lexer {
 public:
    void tokenize(std::string);
    bool match(Token);
    // bool match(std::string);
    Token peek();
    // std::string peek();
    void move();
    bool empty();
};


#endif  // TEAM35_CODE35_SRC_SPA_SRC_SP_LEXER_H_
