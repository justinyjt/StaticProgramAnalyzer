#include <string>
#include "Pattern.h"

class AssignPattern : public Pattern {
private:
    std::string left;
    std::string right;
public:
    AssignPattern(std::string left, std::string right);
};

