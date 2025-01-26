#ifndef READEPIDDATA
#define READEPIDDATA

#include "Person.h"
#include "Dict.h"
#include <stddef.h>
#include <stdio.h>

enum PopulationIsValid{
    EMPTY_INDEX,
    EMPTY_PERSON,
    VALID
};

struct PopulationInfo{
    size_t PopSize;
    size_t WorkSize;
    size_t HHSize;
    size_t SchoolSize;
    enum PopulationIsValid status;
};

struct PopulationInfo ReadPopulation( 
    struct Person*** people, 
    FILE* EpidData,
    DictElement** DictWorks,
    DictElement** DictSchools,
    DictElement** DictHH
);

void PrintPopulationError( struct PopulationInfo ErrorInf );
#endif
