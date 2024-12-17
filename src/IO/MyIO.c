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
    FILE *file = fopen(path, "rb");

    if (!file) {

        errno = EACCES;
        return READ_ERROR;

    }

    *source = file;
    return OK;
}


enum FileStatus OpenWrite(char* path, FILE **source) {
    FILE *file = fopen(path, "wb");

    if (!file) {

        errno = EACCES;
        return WRITE_ERROR;

    }

    *source = file;
    return OK;

}


enum FileStatus CloseFile(FILE* file){

    if (fclose(file) == EOF){
        return CLOSE_ERROR;
    }

    return OK;
}

void PrintIOError( enum FileStatus OpenResult ){
    printf("Error: %s\n", IOError[OpenResult]);
}