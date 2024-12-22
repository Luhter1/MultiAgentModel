#include "Set.h"
#include <uthash.h>
#include <stdio.h>
#include <inttypes.h>
#include <malloc.h>

SetElement* SetGetElement(int id, SetElement* set){
	SetElement *current_user;

	HASH_FIND_INT(set, &id, current_user);

	return current_user;
}

void SetAdd(int id, size_t value, SetElement** set){
	SetElement *current_user = SetGetElement( id, *set );

	if (current_user == NULL) {

		current_user = malloc(sizeof(SetElement));
		current_user->id = id;
		HASH_ADD_INT(*set, id, current_user);
	}
	current_user->value = value;
}

size_t SetLength(SetElement* set){
	SetElement *current_user, *tmp;
	size_t size = 0;

	HASH_ITER(hh, set, current_user, tmp) {

		size++;

	}

	return size;
}

void SetDeleteALL(SetElement* set){
	SetElement *current_user, *tmp;

	HASH_ITER(hh, set, current_user, tmp) {

		HASH_DEL(set,current_user);
		free(current_user);

	}

	free(set);

}