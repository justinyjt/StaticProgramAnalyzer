#pragma once
#include "Tok.h"

/*
 represented by "_"
*/
class Wildcard : public Tok {
  public:
    Wildcard() : Tok::Tok("_",  WILDCARD) {};
};