#include <string>
#include "Term.h"

class Variable : public Term {
public:
    explicit Variable(std::string variableName);

    std::string getVariableName();
private:
    std::string variableName;
};

