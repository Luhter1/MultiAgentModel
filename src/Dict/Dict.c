#include "Dict.h"
#include <uthash.h>
#include <stdio.h>
#include <inttypes.h>
#include <malloc.h>

DictElement* DictGetElement(int id, DictElement* set){
	DictElement *current_user;

	HASH_FIND_INT(set, &id, current_user);

	return current_user;
}

void DictAdd(int id, size_t value, DictElement** set){
	DictElement *current_user = DictGetElement( id, *set );

	if (current_user == NULL) {

		current_user = malloc(sizeof(DictElement));
		current_user->id = id;
		HASH_ADD_INT(*set, id, current_user);
	}
	current_user->value = value;
}

size_t DictLength(DictElement* set){
	DictElement *current_user, *tmp;
	size_t size = 0;

	HASH_ITER(hh, set, current_user, tmp) {

		size++;

	}

	return size;
}

void DictDeleteALL(DictElement* set){
	DictElement *current_user, *tmp;

	HASH_ITER(hh, set, current_user, tmp) {

		HASH_DEL(set,current_user);
		free(current_user);

	}

	free(set);

}