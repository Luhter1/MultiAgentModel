#ifndef EPIDPROCESSES
#define EPIDPROCESSES

#include "Person.h"
#include <stddef.h>

void ChooseInitialInfected( struct Person** people, size_t PopSize, size_t InitCount );

#endif