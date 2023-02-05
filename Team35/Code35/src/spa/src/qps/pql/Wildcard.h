#include "Tok.h"
#include "Arg.h"

/*
 represented by "_"
*/
class Wildcard : public Tok, public StmtRef, public EntRef {};