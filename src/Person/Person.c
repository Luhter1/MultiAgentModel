#include "Person.h"
#include "Utils.h"
#include <string.h>
#include <stddef.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>

#define NULL_INDEX -1
#define ERROR_INDEX -2

struct Person* PersonCreate(
    int32_t id,
    int32_t work_id,
    int32_t school_id,
    int32_t household_id
){
    struct Person* person = malloc( sizeof(struct Person) );

    person->id = id;
    person->work_id = work_id;
    person->school_id = school_id;
    person->household_id = household_id;
    person->IllDay = 0;
    person->state = SUSCEPTIBLE;
    
    return person;
}



static int32_t CheckStrId( char* StrId ){
    long res; 

    if( strcmp(StrId, "") == 0 ) return NULL_INDEX;

    if( GetLong( StrId, &res ) == false ) return ERROR_INDEX;

    return (int32_t)res;
}



struct Person* PersonFromString(
    char* line,
    int32_t id,
    size_t hh_index,
    size_t school_index,
    size_t work_index
){
    char *token;
    int32_t work_id = ERROR_INDEX;
    int32_t school_id = ERROR_INDEX;
    int32_t household_id = ERROR_INDEX;
    size_t index = 0;


    if( strchr( line, '\n' ) != NULL ) line = strsep( &line, "\n" );

    token = strsep( &line, "," );

    while(token != NULL){
        if( index == hh_index ){

            household_id = CheckStrId(token);

        }else if( index == school_index ){

            school_id = CheckStrId(token);

        }else if( index == work_index ){

            work_id = CheckStrId(token);

        }

        token = strsep( &line, "," );
        index++;
    }

    if( 
        work_id != ERROR_INDEX && 
        school_id != ERROR_INDEX && 
        household_id != ERROR_INDEX 
    ){

        return PersonCreate( id, work_id, school_id, household_id );  

    } 

    return NULL;
}



void PersonDelete(struct Person* person){
    free( person );
}