#include "Person.h"
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void ChooseInitialInfected( struct Person** people, size_t PopSize, size_t InitCount ){
    size_t ind;
    
    srand(time(NULL));
 
    for(ind=0; ind<InitCount; ind++){

        int InfIndex = rand() % PopSize;
        people[InfIndex]->state = ILL;

    }

}