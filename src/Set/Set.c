#include "Set.h"
#include <uthash.h>
#include <stdio.h>


void SetAdd(int id, int value, SetElement** set){
    SetElement *current_user;

    HASH_FIND_INT(*set, &id, current_user);

    if (current_user == NULL) {

        current_user = malloc(sizeof(SetElement));
        current_user->id = id;
        current_user->value = value;
        HASH_ADD_INT(*set, id, current_user);
        
    }

}

void SetToArray(SetElement* set){
    SetElement *current_user, *tmp;

    HASH_ITER(hh, set, current_user, tmp) {

        printf("%d ", current_user->id);

    }

    puts("");
}

void SetDeleteALL(SetElement* set){
  SetElement *current_user, *tmp;

  HASH_ITER(hh, set, current_user, tmp) {

    HASH_DEL(set,current_user);
    free(current_user);

  }

  free(set);

}