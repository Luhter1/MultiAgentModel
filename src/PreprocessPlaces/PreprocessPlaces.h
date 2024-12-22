#ifndef PREPROCESSPLACES
#define PREPROCESSPLACES

#include "Set.h"
#include "PeopleLinkedList.h"
#include <stddef.h>

struct PeopleLinkedList** CreatePlace( size_t PlaceCount );

void UpdatePlaces( 
    struct Person** people, 
    size_t PopSize, 
    struct PeopleLinkedList** work, 
    struct PeopleLinkedList** household, 
    struct PeopleLinkedList** school, 
    SetElement* SetHH,
    SetElement* SetWorks,
    SetElement* SetSchools 
);

#endif