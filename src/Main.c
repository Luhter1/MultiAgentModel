#include "Utils.h"
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


// multi_agent_model alpha lmbd init_inf dur ./path_to_file ./path_to_res
int main( int argc, char** argv ) {
    // параметра из командной строки
    double alpha, lmbd;
    size_t InitInfCount, duration;
    // список связных списков
    struct PeopleLinkedList **work, **household, **school;
    // список person
    struct Person** people; 
    // данные для чтения/записи
    FILE *EpidData, *ToWrite;
    // множество работ
    SetElement *SetWorks = NULL;
    // множество школ
    SetElement *SetSchools = NULL;
    // множество домов
    SetElement *SetHH = NULL;
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

    if( ValRes != SUCCESS ){
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

    /*
    Надо:
        x создаем массив из пустых связных списков в размере домов
        x создаем массив из пустых связных списков в размере школ
        x создаем массив из пустых связных списков в размере работ
        x выбрать InitInfected 
        x заполнить множества мест людьми
        x создать связный список больных
        x проходиться каждый день по списку больных
        x добавить имунных
        x задавать alpha, lmbd, InitInfCount, modeling duration, 
        x проверка, что InitInfCount <= PopSize 
            (вообще это неразумные значения 
             и в эпид моделировании не указывают такие большие числа 
             так как эпидемия мгновенно заканчивается и при намного меньших долях)
        x добавить lmbd в процесс заражения
        - сделать --help и man
        x сделать запиcь incidence значений в файл
        - рефакторинг кода
        - документация
        x вывод ошибок в stderr
        - нормальные логи в stdout

    */

    /*----------- создание списка первоначально больных и наделение иммунитетом людей ----------*/
    PreparePeopleState( 
        people, 
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

    /*----------- открыть файл для записи incidence -----------*/
    OpenResult = OpenWrite( argv[6], &ToWrite );

    if( OpenResult != OK )  PrintIOError( OpenResult );

    /*------------ запуск процесса заражения гриппом ------------*/
    Disease( people, household, work, school, &IllPeople, duration, lmbd, ToWrite  );

    if( OpenResult == OK ) CloseFile( ToWrite );

    // if( !IllPeople ) puts("None of Illness");
    
    puts("Successfully done!");

    DeleteArray( people, PopInfo.PopSize);
    return 0;
}