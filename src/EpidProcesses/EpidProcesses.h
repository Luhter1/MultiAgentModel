#ifndef EPIDPROCESSES
#define EPIDPROCESSES

#include "Person.h"
#include "PeopleLinkedList.h"
#include <stddef.h>

void PreparePeopleState( 
    struct Person** people, 
    size_t PopSize, 
    struct PeopleLinkedList** IllPeople,
    size_t InitCount,
    double alpha
);

void Disease(
    struct Person** people,
    struct PeopleLinkedList** household,
    struct PeopleLinkedList** work, 
    struct PeopleLinkedList** school,
    struct PeopleLinkedList** IllPeople,
    size_t duration,
    double lmbd,
    FILE* ToWrite
);

#endif