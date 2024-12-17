#include "ReadPopulation.h"
#include "PeopleLinkedList.h"
#include "Person.h"
#include "Set.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>


#define MIN(a,b) ( ( (a) < (b) ) ? (a) : (b) )
#define MAX(a,b) ( ( (a) > (b) ) ? (a) : (b) )
#define NULL_INDEX -1

// передается ссылка на открытый файл, выдается информация о прочитанной популяции
static const char* PopError[] = {
    [EMPTY_INDEX] = "Not all the indexes were found in",
    [EMPTY_PERSON] = "Cannot create person in line - ",
};

struct IdIndex{
    size_t sp_hh_id;
    size_t sp_school_id;
    size_t sp_work_id;
};

const struct IdIndex InvalidInd = { -1 };

const char* Identifier[] = {"sp_hh_id", "sp_school_id", "sp_work_id"};



static void CheckIndex( char* str, struct IdIndex* ids, size_t index ){

    if(strchr(str, '\n') != NULL ) str = strtok( str, "\n");

    if( strcmp(str, Identifier[0]) == 0 ){

        ids->sp_hh_id = index;

    }else if( strcmp(str, Identifier[1]) == 0 ){
        
        ids->sp_school_id = index;

    }else if( strcmp(str, Identifier[2]) == 0 ){
        
        ids->sp_work_id = index;

    }

}



static enum IsValid validate( struct IdIndex* ids ){
    if( InvalidInd.sp_hh_id == ids->sp_hh_id ) return EMPTY_INDEX;
    else if( InvalidInd.sp_school_id == ids->sp_school_id ) return EMPTY_INDEX;
    else if( InvalidInd.sp_work_id == ids->sp_work_id ) return EMPTY_INDEX;

    return VALID;
}



static enum IsValid GetIdIndex( char* header, struct IdIndex* ids ){
    char *token = strtok( header, ",");
    size_t index = 0;

    while (token != NULL) {
        CheckIndex( token, ids, index );
        token = strtok( NULL, "," );
        index++;
    }

    return validate( ids );
}



static void UpdatePlacec( 
    struct Person* person, 
    SetElement** SetHH, 
    SetElement** SetWorks, 
    SetElement** SetSchools
){

    if( person->household_id != NULL_INDEX ) SetAdd( person->household_id, 0, SetHH );
    
    if( person->school_id != NULL_INDEX ) SetAdd( person->school_id, 0, SetSchools );
    
    if( person->work_id != NULL_INDEX ) SetAdd( person->work_id, 0, SetWorks );

}



struct PopulationInfo ReadPopulation( 
    struct Person** people, 
    FILE* EpidData,
    SetElement** SetWorks,
    SetElement** SetSchools,
    SetElement** SetHH
){
    char line[1024];
    struct PeopleLinkedList* population;
    size_t PersonId = 0;
    struct IdIndex ids = {-1};
    struct PopulationInfo PopInf = {0};

    if( 
        fgets(line, sizeof(line), EpidData) == NULL || 
        GetIdIndex( line, &ids ) != VALID 
    ){

        PopInf.status = EMPTY_INDEX;
        return PopInf;

    }


    while( fgets(line, sizeof(line), EpidData) != NULL ){

        struct Person* curr = PersonFromString( 
            line, 
            PersonId++, 
            ids.sp_hh_id, 
            ids.sp_school_id,
            ids.sp_work_id
        );

        if( curr == NULL){
            PopInf.size = --PersonId;
            PopInf.status = EMPTY_PERSON;
            return PopInf;

        }

        UpdatePlacec( curr, SetHH, SetWorks, SetSchools );

        PopAddFront( &population, curr );
    }
    
    PopInf.status = VALID;
    PopInf.size = population->person->id + 1;

    *people = PopToArray(population, PopInf.size);
    
    PopDestroy( population );

    return PopInf;
}



void PrintPopError( struct PopulationInfo ErrorInf ){
    printf("Error: %s", PopError[ErrorInf.status]);

    if( ErrorInf.status == EMPTY_PERSON ){
        printf("%zu", ErrorInf.size+1);
    }
    
    puts("");

    errno = 1;
}