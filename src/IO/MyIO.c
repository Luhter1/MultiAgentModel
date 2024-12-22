#include "MyIO.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

static const char* const IOError[] = {
    [READ_ERROR] = "Cannot read the epid file",
    [WRITE_ERROR] = "Cannot write results to the file",
    [CLOSE_ERROR] = "Cannot close the epid file",
};

enum FileStatus OpenRead(char* path, FILE **source){
    *source = fopen(path, "rb");

    if( !(*source) ){

        errno = EACCES;
        return READ_ERROR;

    }

    return OK;
}


enum FileStatus OpenWrite(char* path, FILE **source) {
    *source = fopen(path, "wb");

    if( !(*source) ){

        errno = EACCES;
        return WRITE_ERROR;

    }

    return OK;

}


enum FileStatus CloseFile(FILE* file){

    if (fclose(file) == EOF){
        return CLOSE_ERROR;
    }

    return OK;
}


void PrintIOError( enum FileStatus OpenResult ){
    fprintf(stderr, "Error: %s\n\n", IOError[OpenResult]);
}