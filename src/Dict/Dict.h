#ifndef SET
#define SET

#include <uthash.h>

typedef struct {
    int id;
    size_t value;
    UT_hash_handle hh;
} DictElement;

DictElement* DictGetElement(int id, DictElement* set);

void DictAdd(int id, size_t value, DictElement** set);

size_t DictLength(DictElement* set);

void DictDeleteALL(DictElement* set);

#endif