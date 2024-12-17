#ifndef SET
#define SET

#include <uthash.h>

typedef struct {
    int id;
    size_t value;
    UT_hash_handle hh;
} SetElement;

SetElement* SetGetElement(int id, SetElement* set);

void SetAdd(int id, size_t value, SetElement** set);

size_t SetLength(SetElement* set);

void SetDeleteALL(SetElement* set);

#endif