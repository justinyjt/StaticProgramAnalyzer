#include <vector>
#include <string>
#include "qps/clauses/Clause.h"

class QueryParser {
public:
    void setLexer(Lexer& lexer);
    std::pair<std::string , std::vector<Clause>> parse();
private:
    Lexer lexer;
};
