#include "PreprocessPlaces.h"
#include "Set.h"
#include "PeopleLinkedList.h"
#include <stddef.h>

#include <stdio.h>
#include <inttypes.h>

struct PeopleLinkedList** CreatePlace( size_t PlaceCount ){
    struct PeopleLinkedList** NewPlace;
    size_t ind;

    NewPlace = malloc( sizeof(struct PeopleLinkedList*) * PlaceCount );

    for(ind=0; ind<PlaceCount; ind++){
        NewPlace[ind] = NULL;
    }

    return NewPlace;
}

void UpdatePlaces( 
    struct Person** people, 
    size_t PopSize, 
    struct PeopleLinkedList** work, 
    struct PeopleLinkedList** household, 
    struct PeopleLinkedList** school, 
    SetElement* SetHH,
    SetElement* SetWorks,
    SetElement* SetSchools 
){
    size_t ind;
    SetElement* curr;

    for(ind=0; ind<PopSize; ind++){

        struct Person* CurrPerson = people[ind];

        if( CurrPerson->household_id != -1 ){

            curr = SetGetElement( people[ind]->household_id, SetHH );
            people[ind]->household_id = curr->value;
            PopAddFront( household + curr->value, people[ind] );
        }

        if( CurrPerson->work_id != -1 ){

            curr = SetGetElement( people[ind]->work_id, SetWorks );
            people[ind]->work_id = curr->value;
            PopAddFront( work + curr->value, people[ind] );
        }

        if( CurrPerson->school_id != -1 ){

            curr = SetGetElement( people[ind]->school_id, SetSchools );
            people[ind]->school_id = curr->value;
            PopAddFront( school + curr->value, people[ind] );
        }
    }

}