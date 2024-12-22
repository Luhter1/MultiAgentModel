#include "Utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

// const char* Help = "Try --help to see info";


const char* ValidationError[] = {
    [FILE_ACCESS_ERROR] = "Cannot access the file",
    [ARG_NUM_ERROR] = "Incorrect argument format, try --help",
    [ALPHA_ERROR] = "Incorect alpha value",
    [LMBD_ERROR] = "Incorect lambda value",
    [INF_COUNT_ERROR] = "Incorect count of initial infected people",
    [DUR_ERROR] = "Incorect modeling duration value",
};


// void CheckHelp( int argc, char** argv ){
//     if( argc != 1) return;

// }


bool GetDouble( char* str, double* num ){
    char *endptr;
    *num = strtod(str, &endptr);

    if (*endptr != '\0') return false;

    return true;
}


bool GetLong( char* str, long* num ){
    char *endptr;
    *num = strtol(str, &endptr, 10);

    if (*endptr != '\0') return false;

    return true;
}


enum Validation validate( 
    int argc, 
    char** argv,
    double* alpha,
    double* lmbd,
    size_t* InitInfCount,
    size_t* duration
){

    if( argc != 7 ){

        errno = 22;
        return ARG_NUM_ERROR;

    }

    if( GetDouble( argv[1], alpha ) == false ){

        errno = 22;
        return ALPHA_ERROR;

    }

    if( GetDouble( argv[2], lmbd ) == false ){

        errno = 22;
        return LMBD_ERROR;

    }

    if( GetLong( argv[3], InitInfCount ) == false ){

        errno = 22;
        return INF_COUNT_ERROR;

    }

    if( GetLong( argv[4], duration ) == false ){
        
        errno = 22;
        return DUR_ERROR;

    }


    if (access(argv[5], F_OK) != 0) {

        return FILE_ACCESS_ERROR;
        
    }

    return SUCCESS;
}

void PrintValidationError( enum Validation ValidResult ){
    fprintf(stderr, "Error: %s\n", ValidationError[ValidResult]);
}
