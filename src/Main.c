#include "Utils.h"
#include "MyIO.h"
#include "Person.h"
#include "PeopleLinkedList.h"
#include "ReadPopulation.h"
#include "PreprocessPlaces.h"
#include "EpidProcesses.h"
#include "Dict.h"
#include <stdio.h>
#include <errno.h>
#include <inttypes.h>

static void DestroySet(
    DictElement *DictWorks, 
    DictElement *DictSchools, 
    DictElement *DictHH
){
    DictDeleteALL( DictSchools );
    DictDeleteALL( DictHH );
    DictDeleteALL( DictWorks );
}



int main( int argc, char** argv ) {
    // параметра из командной строки
    double alpha, lmbd;
    size_t InitInfCount, duration;
    // список связных списков
    struct PeopleLinkedList **work, **household, **school;
    // список person
    struct Person** population; 
    // данные для чтения/записи
    FILE *EpidData, *ToWrite;
    // множество работ
    DictElement *DictWorks = NULL;
    // множество школ
    DictElement *DictSchools = NULL;
    // множество домов
    DictElement *DictHH = NULL;
    // связный список 
    struct PeopleLinkedList* IllPeople = NULL;

    /*-------- валидация входных параметров -------*/
    enum Validation ValRes = validate( 
        argc, 
        argv, 
        &alpha, 
        &lmbd, 
        &InitInfCount, 
        &duration 
    );

    if( ValRes == HELP ){
        PrintHelp();
        return errno;
    }else if( ValRes != SUCCESS ){
        PrintValidationError( ValRes );
        return errno;
    }

    /*----------- открытие файла для чтения ----------*/
    enum FileStatus OpenResult = OpenRead( argv[5], &EpidData );

    if( OpenResult != OK ){
        PrintIOError( OpenResult );
        return errno;
    }


    /*----------- создание популяции и множества мест заражений ----------*/
    struct PopulationInfo PopInfo = ReadPopulation( 
        &population, 
        EpidData, 
        &DictWorks, 
        &DictSchools, 
        &DictHH 
    );

    CloseFile( EpidData );

    if( PopInfo.status != VALID ){
        DestroySet(DictWorks, DictSchools, DictHH);
        PrintPopulationError( PopInfo );
        return errno;
    }


    /*----------- создание списка первоначально больных и наделение иммунитетом людей ----------*/
    PreparePeopleState( 
        population, 
        PopInfo.PopSize, 
        &IllPeople, 
        InitInfCount,
        alpha
    );


    /*----------- создание списка мест заражения ----------*/
    work = CreatePlace( PopInfo.WorkSize );
    household = CreatePlace( PopInfo.HHSize );
    school = CreatePlace( PopInfo.SchoolSize );

    UpdatePlaces( 
        population, 
        PopInfo.PopSize, 
        work, 
        household, 
        school, 
        DictHH,
        DictWorks,
        DictSchools 
    );

    DestroySet( DictWorks, DictSchools, DictHH );


    /*----------- открыть файл для записи incidence -----------*/
    OpenResult = OpenWrite( argv[6], &ToWrite );

    if( OpenResult != OK )  PrintIOError( OpenResult );


    /*------------ запуск процесса заражения гриппом ------------*/
    Disease( population, household, work, school, &IllPeople, duration, lmbd, ToWrite  );

    if( OpenResult == OK ) CloseFile( ToWrite );
    
    puts("Successfully done!");
    DeleteArray( population, PopInfo.PopSize);

    return 0;
}