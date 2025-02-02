#include "ReadPopulation.h"
#include "PeopleLinkedList.h"
#include "Person.h"
#include "Dict.h"
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



static enum PopulationIsValid PopulationValidate( struct IdIndex* ids ){
    if( InvalidInd.sp_hh_id == ids->sp_hh_id ) return EMPTY_INDEX;
    else if( InvalidInd.sp_school_id == ids->sp_school_id ) return EMPTY_INDEX;
    else if( InvalidInd.sp_work_id == ids->sp_work_id ) return EMPTY_INDEX;

    return VALID;
}



static enum PopulationIsValid GetIdIndex( char* header, struct IdIndex* ids ){
    char *token = strtok( header, ",");
    size_t index = 0;

    while (token != NULL) {
        CheckIndex( token, ids, index );
        token = strtok( NULL, "," );
        index++;
    }

    return PopulationValidate( ids );
}



static void UpdatePlacec( 
    struct Person* person, 
    DictElement** DictHH, 
    DictElement** DictWorks, 
    DictElement** DictSchools,
    size_t* HHCount, 
    size_t* WorkCount,
    size_t* SchoulCount
){

    if( person->household_id != NULL_INDEX ){

        if(DictGetElement( person->household_id, *DictHH ) == NULL){
            DictAdd( person->household_id, *HHCount, DictHH );
            (*HHCount)++;
        }

    }

    if( person->school_id != NULL_INDEX ){

        if(DictGetElement( person->school_id, *DictSchools ) == NULL){
            DictAdd( person->school_id, *SchoulCount, DictSchools );
            (*SchoulCount)++;
        }

    }

    if( person->work_id != NULL_INDEX ){

        if(DictGetElement( person->work_id, *DictWorks ) == NULL){
            DictAdd( person->work_id, *WorkCount, DictWorks );
            (*WorkCount)++;
        }

    }
}



struct PopulationInfo ReadPopulation( 
    struct Person*** population, 
    FILE* EpidData,
    DictElement** DictWorks,
    DictElement** DictSchools,
    DictElement** DictHH
){
    char line[1024];
    struct PeopleLinkedList* LinkedPopulation;
    size_t PersonId = 0;
    struct IdIndex ids = {-1};
    struct PopulationInfo PopInf = {0};
    size_t HHCount = 0, WorkCount = 0, SchoulCount = 0;

    // читаем первую строку файла и получаем индексы параметров персоны 
    if( 
        fgets(line, sizeof(line), EpidData) == NULL || 
        GetIdIndex( line, &ids ) != VALID 
    ){

        PopInf.status = EMPTY_INDEX;
        return PopInf;

    }

    // читаем построчно файл, добавляя в связный список новых людей
    // обновляем множества мест заражения
    while( fgets(line, sizeof(line), EpidData) != NULL ){

        struct Person* curr = PersonFromString( 
            line, 
            PersonId++, 
            ids.sp_hh_id, 
            ids.sp_school_id,
            ids.sp_work_id
        );

        if( curr == NULL){

            PopInf.PopSize = --PersonId;
            PopInf.status = EMPTY_PERSON;
            return PopInf;

        }

        UpdatePlacec(
            curr, 
            DictHH, 
            DictWorks, 
            DictSchools, 
            &HHCount, 
            &WorkCount,
            &SchoulCount
        );

        PopAddFront( &LinkedPopulation, curr );
    }


    PopInf.status = VALID;
    PopInf.PopSize = LinkedPopulation->person->id + 1;
    PopInf.WorkSize = WorkCount;
    PopInf.HHSize = HHCount;
    PopInf.SchoolSize = SchoulCount;

    // преобразуем связный список людей в массив 
    // для быстрой адресации к элементам
    *population = PopToArray( LinkedPopulation, PopInf.PopSize );

    // удаляем связный список
    PopDestroy( LinkedPopulation, PopInf.PopSize );
    
    return PopInf;
}



void PrintPopulationError( struct PopulationInfo ErrorInf ){

    fprintf(stderr, "Error: %s", PopError[ErrorInf.status]);

    if( ErrorInf.status == EMPTY_PERSON ){
        fprintf(stderr, "%zu", ErrorInf.PopSize+1);
    }
    
    fprintf(stderr, "\n");

    errno = 1;
}