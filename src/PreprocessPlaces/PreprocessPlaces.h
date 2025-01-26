#ifndef PREPROCESSPLACES
#define PREPROCESSPLACES

#include "Dict.h"
#include "PeopleLinkedList.h"
#include <stddef.h>

struct PeopleLinkedList** CreatePlace( size_t PlaceCount );

void UpdatePlaces( 
    struct Person** people, 
    size_t PopSize, 
    struct PeopleLinkedList** work, 
    struct PeopleLinkedList** household, 
    struct PeopleLinkedList** school, 
    DictElement* DictHH,
    DictElement* DictWorks,
    DictElement* DictSchools 
);

#endif