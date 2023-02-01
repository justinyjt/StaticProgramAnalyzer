#ifndef SPA_TOKEN_H
#define SPA_TOKEN_H

#include <string>

class Token {
public:
    std::string getTokenType();
    std::string getStringValue();

private:
    std::string tokenType;
    std::string value;
};
#endif //SPA_TOKEN_H
