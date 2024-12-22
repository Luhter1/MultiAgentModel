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


void PopDestroy( struct PeopleLinkedList* list, size_t size ) {
    size_t ind;
    struct PeopleLinkedList* to_del;

    if( !list ) return;

    for(ind=0; ind<size; ind++){
        to_del = list;
        list = list -> next; 
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



void PopRemoveElement( 
    struct PeopleLinkedList** people,
    struct PeopleLinkedList* PrevPerson, 
    struct PeopleLinkedList** CurrPerson
){
    size_t ind;
    struct PeopleLinkedList* ToDel = *CurrPerson;

    if( *CurrPerson == NULL ) return;

    if( PrevPerson != NULL ){

        PrevPerson->next = (*CurrPerson)->next;

    }else{

        *people = (*CurrPerson)->next;

    }

    *CurrPerson = (*CurrPerson)->next;
    free( ToDel );
}



struct Person** PopToArray(struct PeopleLinkedList* list, size_t size){
    size_t ind;
    struct Person** people = malloc( sizeof(struct Person*) * size );

    for(ind=size; 0<ind; ind--){
        people[ind-1] = list->person;
        list = list -> next; 

    }
    // people[0] = list->person;

    return people;
}



void DeleteArray(struct Person** people, size_t size){
    size_t ind;

    for(ind=0; ind<size; ind++){
        free(people[ind]);
    }

    free(people);
}
