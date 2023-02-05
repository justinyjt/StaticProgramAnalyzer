#ifndef TEAM35_CODE35_SRC_SPA_SRC_COMMONS_TOKEN_H_
#define TEAM35_CODE35_SRC_SPA_SRC_COMMONS_TOKEN_H_

#include <string>

class Token {
 public:
    std::string getTokenType();
    std::string getStringValue();

 private:
    std::string tokenType;
    std::string value;
};


#endif  // TEAM35_CODE35_SRC_SPA_SRC_COMMONS_TOKEN_H_
