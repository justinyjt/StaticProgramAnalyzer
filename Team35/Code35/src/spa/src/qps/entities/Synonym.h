#pragma once

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
    bool operator==(Synonym const &rhs) const;

 private:
    DesignEntity synonym;
    std::string declaration;
};

