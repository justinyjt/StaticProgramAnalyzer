#include "Relationship.h"

class Modify : public Relationship {
private:
    int arg1;
    std::string arg2;
public:
    Modify(int, std::string);
};
