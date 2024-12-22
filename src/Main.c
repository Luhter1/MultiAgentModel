#include "MyIO.h"
#include "Person.h"
#include "PeopleLinkedList.h"
#include "ReadPopulation.h"
#include "PreprocessPlaces.h"
#include "EpidProcesses.h"
#include "Set.h"
#include <stdio.h>
#include <errno.h>
#include <inttypes.h>

static void DestroySet(
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
    struct PeopleLinkedList **work, **household, **school;
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
    struct PopulationInfo PopInfo = ReadPopulation( 
        &people, 
        EpidData, 
        &SetWorks, 
        &SetSchools, 
        &SetHH 
    );

    CloseFile( EpidData );

    if( PopInfo.status != VALID ){
        DestroySet(SetWorks, SetSchools, SetHH);
        PrintPopError( PopInfo );
        return errno;
    }

    /*
    Есть:
        - массив людей
        - множество работ
        - множество школ
        - множество домов
    */
    // people, &SetWorks, &SetSchools, &SetHH
    /*
    Надо:
        x создаем массив из пустых связных списков в размере домов
        x создаем массив из пустых связных списков в размере школ
        x создаем массив из пустых связных списков в размере работ
        x выбрать InitInfected 
        x заполнить множества мест людьми
        - создать связный список больных
        - добавить имунных
        - задавать InitInfCount
    */

    ChooseInitialInfected( people, PopInfo.PopSize, 10 );

    // /*----------- создание списка мест заражения и списка первоначально больных ----------*/
    work = CreatePlace( PopInfo.WorkSize );
    household = CreatePlace( PopInfo.HHSize );
    school = CreatePlace( PopInfo.SchoolSize );

    UpdatePlaces( 
        people, 
        PopInfo.PopSize, 
        work, 
        household, 
        school, 
        SetHH,
        SetWorks,
        SetSchools 
    );

    DestroySet( SetWorks, SetSchools, SetHH );
    
    // printf("%" PRId32 "\n", work[0]->person->id);
    // printf("%d\n", work[141]->person->state);
    // printf("%" PRId32 "\n", people[0]->state);
    // printf("%" PRId32 "\n", people[2]->state);


    DeleteArray( people, PopInfo.PopSize);
    return 0;
}