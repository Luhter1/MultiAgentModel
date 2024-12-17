#ifndef SET
#define SET

#include <uthash.h>

typedef struct {
    int id;
    int value;
    UT_hash_handle hh;
} SetElement;

void SetAdd(int id, int value, SetElement** set);

void SetToArray(SetElement* set);

void SetDeleteALL(SetElement* set);

#endif