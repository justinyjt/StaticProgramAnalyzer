#include <string>
#include "Term.h"

class Synonym : public Term {
public:
    enum class DesignEntity {
        Variable, Assign,
    };

    Synonym(DesignEntity synonym, std::string declaration);

    DesignEntity getDesignEntity();
    std::string getDeclaration();

private:
    DesignEntity synonym;
    std::string declaration;
};

