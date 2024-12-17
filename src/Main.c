#include "MyIO.h"
#include "Person.h"
#include "PeopleLinkedList.h"
#include "ReadPopulation.h"
#include "Set.h"
#include <stdio.h>
#include <errno.h>
#include <inttypes.h>

static void destroy(
    SetElement *SetWorks, 
    SetElement *SetSchools, 
    SetElement *SetHH
){
    SetDeleteALL( SetSchools );
    SetDeleteALL( SetHH );
    SetDeleteALL( SetWorks );
}


// На текущий момент реализованно создание списка людей 
// и множества мест заражений
// осталось преобразовать множество мест заражений в списки связных списков людей
// далее уже будет использован мультиагентный алгоритм заражения гриппом
int main( int argc, char** argv ) {
    // множество работ
    SetElement *SetWorks = NULL;
    // множество школ
    SetElement *SetSchools = NULL;
    // множество домов
    SetElement *SetHH = NULL;
    // список связных списков
    struct PeopleLinkedList* work, household, school;
    // список person
    struct Person** people; 
    // связный список 
    struct PeopleLinkedList* illnesses;
    // данные для чтения
    FILE* EpidData;

    /*----------- открытие файла для чтения ----------*/
    enum FileStatus OpenResult = OpenRead(argv[1], &EpidData);

    if( OpenResult != OK ){
        PrintIOError( OpenResult );
        return errno;
    }


    /*----------- создание популяции и множества мест заражений ----------*/
    struct PopulationInfo PopInfo = ReadPopulation( &people, EpidData, &SetWorks, &SetSchools, &SetHH );

    // CloseFile( EpidData );

    if( PopInfo.status != VALID ){
        destroy(SetWorks, SetSchools, SetHH);
        PrintPopError( PopInfo );
        return errno;
    }


    /*----------- создание списка мест заражения и списка первоначально больных ----------*/
    // people, &SetWorks, &SetSchools, &SetHH

    printf("%" PRId32 "\n", people[0]->id);
    printf("%zu\n", SetGetElement( 500143905, SetWorks )->value);

    DeleteArray( people, PopInfo.size);
    destroy( SetWorks, SetSchools, SetHH );
    return 0;
}