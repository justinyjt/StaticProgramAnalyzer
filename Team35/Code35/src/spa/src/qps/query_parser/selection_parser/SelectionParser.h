#include <vector>

class SelectionParser {
public:
    std::string parse(Lexer &lexer, std::vector<std::string> synonyms);
};

