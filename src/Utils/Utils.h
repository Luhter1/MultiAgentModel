#ifndef UTILS
#define UTILS

#include <stdbool.h>
#include <stddef.h>

enum Validation  {
        SUCCESS,
        HELP,   
        FILE_ACCESS_ERROR,  
        ARG_NUM_ERROR,
        ALPHA_ERROR,
        LMBD_ERROR,
        INF_COUNT_ERROR,
        DUR_ERROR
};

bool GetDouble( char* str, double* num );

bool GetLong( char* str, long* num );

enum Validation validate( 
    int argc, 
    char** argv,
    double* alpha,
    double* lmbd,
    size_t* InitInfCount,
    size_t* duration
);

void PrintHelp();

void PrintValidationError( enum Validation ValidResult );

#endif /* Utils.h */
