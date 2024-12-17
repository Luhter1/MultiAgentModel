#ifndef IO
#define IO 1

#include <stdio.h>

enum FileStatus  {
        OK,
        READ_ERROR,
        WRITE_ERROR,
        CLOSE_ERROR
};


enum FileStatus OpenRead(char* path, FILE **source);

enum FileStatus OpenWrite(char* path, FILE **source);

enum FileStatus CloseFile(FILE* file);

void PrintIOError( enum FileStatus OpenResult );

#endif /* iostat.h */
