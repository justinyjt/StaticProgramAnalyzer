#include <vector>
#include "qps/entities/Synonym.h"

class SelectionParser {
public:
    Synonym parse(std::unique_ptr<Lexer> lexer, std::vector<Synonym> synonyms);
};

