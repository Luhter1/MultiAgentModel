#include "Person.h"
#include "PeopleLinkedList.h"
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include <stdio.h>

#define round( num ) (int)(num < 0 ? (num - 0.5) : (num + 0.5))

static const int BRfunc[] = {0, 0, 90, 90, 55, 30, 15, 5};

static int InfRand( size_t Num ){

    return rand() % Num;

}

void PreparePeopleState( 
    struct Person** people, 
    size_t PopSize, 
    struct PeopleLinkedList** IllPeople,
    size_t InitCount,
    double alpha
){
    size_t ind, ImmuneCount = round( (1 - alpha) * PopSize );

    if( InitCount > PopSize ) InitCount = PopSize;

    srand(time(NULL));
    
    for(ind=0; ind<InitCount; ind++){

        int InfIndex = InfRand( PopSize );
        struct Person* CurrPerson = people[InfIndex];

        if( CurrPerson->state == SUSCEPTIBLE ){

            CurrPerson->IllDay = 3;
            CurrPerson->state = ILL;
            PopAddFront( IllPeople, CurrPerson );

        }else{
            ind--;
        }
    }

    for(ind=0; ind<ImmuneCount; ind++){

        int InfIndex = InfRand( PopSize );
        struct Person* CurrPerson = people[InfIndex];

        if( CurrPerson->state == SUSCEPTIBLE ){

            CurrPerson->state = IMMUNE;

        }else{
            ind--;
        }

    }
}


static size_t HHInfection( 
    struct PeopleLinkedList** IllPeople, 
    struct Person* IllPerson, 
    struct PeopleLinkedList** household,
    int infectivity 
){
    // список сожителей данного больного
    struct PeopleLinkedList* CurrHH = household[ IllPerson->household_id ];
    size_t HHIncidence = 0;

    while( CurrHH ){

        if( CurrHH->person->state == SUSCEPTIBLE && infectivity > InfRand( 101 ) ){
            
            CurrHH->person->state = ILL;
            CurrHH->person->IllDay = 0;
            PopAddFront( IllPeople, CurrHH->person );
            HHIncidence++;

        }
        CurrHH = CurrHH->next;

    }

    return HHIncidence;

}

static size_t WorkInfection( 
    struct PeopleLinkedList** IllPeople, 
    struct Person* IllPerson, 
    struct PeopleLinkedList** work,
    int infectivity  
){
    // список коллег данного больного
    struct PeopleLinkedList* CurrWork = work[ IllPerson->work_id ];
    size_t WorkIncidence = 0;

    while( CurrWork ){

        if( CurrWork->person->state == SUSCEPTIBLE && infectivity > InfRand( 101 ) ){
            
            CurrWork->person->state = ILL;
            CurrWork->person->IllDay = 0;
            PopAddFront( IllPeople, CurrWork->person );
            WorkIncidence++;

        }
        CurrWork = CurrWork->next;

    }

    return WorkIncidence;

}



static size_t SchoolInfection( 
    struct PeopleLinkedList** IllPeople, 
    struct Person* IllPerson, 
    struct PeopleLinkedList** work,
    int infectivity 
){
    // список одноклассников данного больного
    struct PeopleLinkedList* CurrSchool = work[ IllPerson->school_id ];
    // заразность текущего человека
    size_t SchoolIncidence = 0;

    while( CurrSchool ){

        if( CurrSchool->person->state == SUSCEPTIBLE && infectivity > InfRand( 101 ) ){
            
            CurrSchool->person->state = ILL;
            CurrSchool->person->IllDay = 0;
            PopAddFront( IllPeople, CurrSchool->person );
            SchoolIncidence++;

        }
        CurrSchool = CurrSchool->next;

    }

    return SchoolIncidence;

}


static size_t day( 
    struct PeopleLinkedList** IllPeople, 
    struct PeopleLinkedList** household,
    struct PeopleLinkedList** work, 
    struct PeopleLinkedList** school,
    double lmbd
){
    struct PeopleLinkedList* CurrPerson = *IllPeople;
    struct PeopleLinkedList* PrevPerson = NULL;
    size_t Incidence = 0;

    srand(time(NULL));

    while( CurrPerson ){
        
        struct Person* person = CurrPerson->person;
        int infectivity = round( lmbd * BRfunc[ person->IllDay ] );

        // Надо вирулентность умножить на lmbd
        Incidence += HHInfection( IllPeople, person, household, infectivity );

        if( person->work_id != -1 ){
        
            Incidence += WorkInfection( IllPeople, person, work, infectivity );
        
        }
        
        if( person->school_id != -1 ){
        
            Incidence += SchoolInfection( IllPeople, person, school, infectivity );
        
        }
        
        if( ++(person->IllDay) == 8 ){

            person->state = RECOVERED;
            PopRemoveElement( IllPeople, PrevPerson, &CurrPerson );

        }else{

            PrevPerson = CurrPerson;
            CurrPerson = CurrPerson->next;
        
        } 

    }

    return Incidence;

}

void Disease(
    struct Person** people,
    struct PeopleLinkedList** household,
    struct PeopleLinkedList** work, 
    struct PeopleLinkedList** school,
    struct PeopleLinkedList** IllPeople,
    size_t duration,
    double lmbd,
    FILE* ToWrite
){
    size_t CurrDay;

    for(CurrDay=0; CurrDay<duration; CurrDay++){

        size_t Incidence = 0;

        if( *IllPeople != NULL){

            Incidence += day( IllPeople, household, work, school, lmbd );

        }

        if( ToWrite != NULL ) fprintf(ToWrite, "%zu\n", Incidence);
        
        printf("Day: %zu\n", CurrDay);
        printf("Ill count: %zu\n", PopLength( *IllPeople ));
        printf("Incidence: %zu\n", Incidence);
        puts("");

    }

}