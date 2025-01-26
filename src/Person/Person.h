#ifndef PERSON
#define PERSON

#include <inttypes.h>
#include <stddef.h>

enum illness_state{
    SUSCEPTIBLE,
    IMMUNE,
    ILL,
    RECOVERED
};

struct Person{
    int32_t id;
    int32_t work_id;
    int32_t school_id;
    int32_t household_id;
    size_t IllDay;
    enum illness_state state;
};

struct Person* PersonCreate(
    int32_t id,
    int32_t work_id,
    int32_t school_id,
    int32_t household_id
);

struct Person* PersonFromString(
    char* line,
    int32_t id,
    size_t hh_index,
    size_t school_index,
    size_t work_index
);

void PersonDelete(struct Person* person);

#endif
