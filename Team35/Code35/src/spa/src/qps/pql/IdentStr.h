#include <string>
#include "Tok.h"
#include "Arg.h"

/*
represents string identifier
*/
class IdentStr : public Tok, public StmtRef {
  std::string s;
  IdentStr(std::string s) : s(s) {};
};
