#ifndef SPA_LEXER_H
#define SPA_LEXER_H
#include <string>
#include "commons/Token.h"


class Lexer {
public:
    void tokenize(std::string);
    bool match(Token);
    //bool match(std::string);
    Token peek();
    //std::string peek();
    void move();
    bool empty();
};
#endif //SPA_LEXER_H
