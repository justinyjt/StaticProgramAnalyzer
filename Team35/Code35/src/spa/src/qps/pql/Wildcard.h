#include "Tok.h"

/*
 represented by "_"
*/
class Wildcard : public Tok {
  public:
    Wildcard() : Tok(WILDCARD) {};
};