#ifndef PEOPLELINKEDLIST
#define PEOPLELINKEDLIST

#include "Person.h"
#include <stddef.h>

struct PeopleLinkedList {
   struct Person* person;
   struct PeopleLinkedList* next;
};

struct PeopleLinkedList* NodeCreate( struct Person* person );

void PopAddFront( struct PeopleLinkedList** old, struct Person* person );

size_t PopLength( const struct PeopleLinkedList* list );

void PopDestroy( struct PeopleLinkedList* list );

struct PeopleLinkedList* PopLast( struct PeopleLinkedList * list );

void PopAddBack( struct PeopleLinkedList** old, struct Person* person );

struct Person* PopToArray(struct PeopleLinkedList* population, size_t size);

void DeleteArray(struct Person** people, size_t size);

#endif
