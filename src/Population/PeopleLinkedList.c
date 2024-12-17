#include "PeopleLinkedList.h"
#include "Person.h"
#include <inttypes.h>
#include <malloc.h>
#include <stddef.h>
#include <string.h>

struct PeopleLinkedList* NodeCreate( struct Person* person ) {
    struct PeopleLinkedList* res = malloc(sizeof(struct PeopleLinkedList));
    
    res->person = person;
    res->next = NULL;
    
    return res;
}


void PopAddFront( struct PeopleLinkedList** old, struct Person* person ) {
    struct PeopleLinkedList* new = NodeCreate( person );

    new->next = *old;
    *old = new;
}


size_t PopLength( const struct PeopleLinkedList* list ) {
    size_t c = 1;
    
    if( !list ) return 0;
    
    while(list->next){
        list = list->next;
        c++;
    }
    
    return c;
}


void PopDestroy( struct PeopleLinkedList* list ) {
    struct PeopleLinkedList* to_del;
        
    while (list) {
        to_del = list;
        list = list -> next; 
        DeletePerson( to_del->person );
        free( to_del );
    }
}


struct PeopleLinkedList* PopLast( struct PeopleLinkedList * list ) {
    if( !list ) return NULL;
    
    while(list->next){
        list = list->next;
    }
    
    return list;
}


void PopAddBack( struct PeopleLinkedList** old, struct Person* person ) {

    struct PeopleLinkedList* last = PopLast( *old );
    
    if( !last ){ PopAddFront( old, person ); }
    else{ last->next = NodeCreate( person ); }
    
}

struct Person* PopToArray(struct PeopleLinkedList* list, size_t size){
    size_t ind = 0;
    size_t PersonSize = sizeof(struct Person);
    struct Person* people = malloc( sizeof(struct Person) * size );
    while (list) {
        memcpy(people + ind, list->person, PersonSize);
        ind++;
        list = list -> next; 
    }

    return people;
}
