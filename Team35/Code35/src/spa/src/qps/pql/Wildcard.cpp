#pragma once

#include "Tok.h"
#include "Wildcard.h"

Wildcard::Wildcard() : Tok::Tok("_", WILDCARD) {}
