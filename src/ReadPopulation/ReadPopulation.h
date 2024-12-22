#ifndef READEPIDDATA
#define READEPIDDATA

#include "Person.h"
#include "Set.h"
#include <stddef.h>
#include <stdio.h>

enum IsValid{
    EMPTY_INDEX,
    EMPTY_PERSON,
    VALID
};

struct PopulationInfo{
    size_t PopSize;
    size_t WorkSize;
    size_t HHSize;
    size_t SchoolSize;
    enum IsValid status;
};

struct PopulationInfo ReadPopulation( 
    struct Person*** people, 
    FILE* EpidData,
    SetElement** SetWorks,
    SetElement** SetSchools,
    SetElement** SetHH
);

void PrintPopError( struct PopulationInfo ErrorInf );
#endif
