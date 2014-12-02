#ifndef __LIST_H__
#define __LIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helpers functions
// Compare functions
int Int_cmp(int n1, int n2);
int Float_cmp(float n1, float n2);
int Double_cmp(double n1, double n2);
int Ptr_cmp(void * pt1, void * pt2);
int Str_cmp(char * str1, char * str2);

// Copy functions
void Int_copy(int * dest, int * src);
void Float_copy(float * dest, float * src);
void Double_copy(double * dest, double * src);
void Ptr_copy(void ** dest, void ** src);
void Str_copy(char ** dest, char ** src);

// Free functions
void Int_free(int val);
void Float_free(float val);
void Double_free(double val);
void Str_free(char * str);

// =============
//  Definitions
// =============
#define NEW_LIST_ELEM(ElemTypename, Valuetype, Indextype) \
typedef struct _ ## ElemTypename \
{ \
	Valuetype value; \
	Indextype index; \
	struct _ ## ElemTypename * next; \
	struct _ ## ElemTypename * prev; \
} ElemTypename

#define NEW_LIST_TYPE(LIST, Valuetype, Indextype) \
NEW_LIST_ELEM(LIST ## _elem_t, Valuetype, Indextype); \
typedef struct LIST \
{ \
	LIST ## _elem_t * array; \
	LIST ## _elem_t * end; \
	int    size; \
	size_t elemSize; \
	int    freeValue; \
	int    freeIndex; \
	void (*_copyValue)(Valuetype * dest, Valuetype * src); \
	void (*_copyIndex)(Indextype * dest, Indextype * src); \
	int (*_cmpValue)(Valuetype val1, Valuetype val2); \
	int (*_cmpIndex)(Indextype val1, Indextype val2); \
	void (*_freeValue)(Valuetype value); \
	void (*_freeIndex)(Indextype index); \
} LIST

#define LIST_FN_NEW(LIST) \
LIST * LIST ## _new()

#define LIST_FN_FREE(LIST) \
void LIST ## _free(LIST * array)

#define LIST_FN_ADD_STRUCT(LIST, Valuetype, Indextype) \
LIST ## _elem_t * LIST ## _add(LIST * array, Indextype index, Valuetype value)

#define LIST_FN_REMOVE_STRUCT(LIST, Indextype) \
LIST * LIST ## _remove(LIST * array, Indextype index)

#define LIST_FN_GET_STRUCT(LIST, Indextype) \
LIST ## _elem_t * LIST ## _get(LIST * array, Indextype index)

#define LIST_FN_SEARCH_STRUCT(LIST, Valuetype) \
LIST ## _elem_t * LIST ## _search(LIST * array, Valuetype search)

//#define LIST_FN_GET_PREVIOUS(LIST, Indextype) \
//LIST ## _elem_t LIST ## _find_previous(LIST * array, Indextype index)

// =================
//  Implementations
// =================
#define IMPLEMENT_LIST_FN_NEW(LIST, Valuetype, Indextype, FN_CPY_VAL, FN_CPY_IDX, FN_CMP_VAL, FN_CMP_IDX, FN_FREE_VAL, FN_FREE_IDX) \
LIST * LIST ## _new() \
{ \
	LIST * array = malloc(sizeof(LIST)); \
	array->size  = 0; \
	array->array = NULL; \
	array->end   = NULL; \
	array->elemSize   = sizeof(LIST ## _elem_t); \
	array->freeValue  = 1; \
	array->freeIndex  = 1; \
	array->_copyValue = FN_CPY_VAL; \
	array->_copyIndex = FN_CPY_IDX; \
	array->_cmpValue  = FN_CMP_VAL; \
	array->_cmpIndex  = FN_CMP_IDX; \
	array->_freeValue = FN_FREE_VAL; \
	array->_freeIndex = FN_FREE_IDX; \
	return array; \
}

#define IMPLEMENT_LIST_FN_FREE(LIST) \
void LIST ## _free(LIST * array) \
{ \
	LIST ## _elem_t * it; \
	if(array == NULL) return; \
	for(it = array->array ; it != NULL ; it = it->next) \
	{ \
		if(it->prev) free(it->prev); \
		if(array->freeValue) array->_freeValue(it->value); \
		if(array->freeIndex) array->_freeIndex(it->index); \
	} \
	free(array); \
}

#define IMPLEMENT_LIST_FN_ADD_STRUCT(LIST, Valuetype, Indextype) \
LIST ## _elem_t * LIST ## _add(LIST * array, Indextype index, Valuetype value) \
{ \
	LIST ## _elem_t * elem = NULL; \
	/* Test if array is NULL */\
	if(array == NULL) \
		return NULL; \
	/* Test if index exists */\
	if(LIST ## _get(array, index) != NULL) \
		return NULL; \
	/* Create the element */\
	elem = malloc(array->elemSize); \
	elem->prev = NULL; \
	elem->next = NULL; \
	array->_copyIndex(&(elem->index), &(index)); \
	array->_copyValue(&(elem->value), &(value)); \
	/* Insert the element */\
	array->size++; \
	if(array->array == NULL) \
	{ \
		array->array = elem; \
		array->end   = elem; \
	} \
	else \
	{ \
		array->end->next = elem; \
		elem->prev = array->end; \
		array->end = elem; \
	} \
	return elem; \
}


#define IMPLEMENT_LIST_FN_REMOVE_STRUCT(LIST, Indextype) \
LIST * LIST ## _remove(LIST * array, Indextype index) \
{ \
	LIST ## _elem_t * elem = LIST ## _get(array, index); \
	if(elem != NULL) \
	{ \
		if(array->freeValue) array->_freeValue(elem->value); \
		if(array->freeIndex) array->_freeIndex(elem->index); \
		elem->prev->next = elem->next; \
		elem->next->prev = elem->prev; \
		free(elem); \
	} \
	array->size--; \
	return array; \
}


#define IMPLEMENT_LIST_FN_GET_STRUCT(LIST, Indextype) \
LIST ## _elem_t * LIST ## _get(LIST * array, Indextype index) \
{ \
	LIST ## _elem_t * out = NULL; \
	LIST ## _elem_t * it = NULL; \
	/* Check empty array */ \
	if(array == NULL)        return NULL; \
	if(array->array == NULL) return NULL; \
	/* Start search */ \
	it = array->array; \
	while(it != NULL) \
	{ \
		if(array->_cmpIndex(it->index, index) == 0) \
		{ \
			out = it; \
			it  = NULL; \
		} \
		else \
			it = it->next; \
	} \
	return out; \
}


#define IMPLEMENT_LIST_FN_SEARCH_STRUCT(LIST, Valuetype) \
LIST ## _elem_t * LIST ## _search(LIST * array, Valuetype search) \
{ \
	LIST ## _elem_t * out = NULL; \
	LIST ## _elem_t * it = NULL; \
	/* Check empty array */ \
	if(array == NULL)        return NULL; \
	if(array->array == NULL) return NULL; \
	/* Start search */ \
	it = array->array; \
	while(it != NULL) \
	{ \
		if(array->_cmpValue(it->value, search) == 0) \
		{ \
			out == it; \
			it = NULL; \
		} \
		else \
			it = it->next; \
	} \
	return out; \
}

// MACRO HELPERS (One line definitions && implementations)
#define NEW_LIST_DEFINITION(LIST, VALUETYPE, INDEXTYPE) \
NEW_LIST_TYPE(LIST, VALUETYPE, INDEXTYPE); \
LIST_FN_NEW(LIST); \
LIST_FN_FREE(LIST); \
LIST_FN_ADD_STRUCT(LIST, VALUETYPE, INDEXTYPE); \
LIST_FN_REMOVE_STRUCT(LIST, INDEXTYPE); \
LIST_FN_GET_STRUCT(LIST, INDEXTYPE); \
LIST_FN_SEARCH_STRUCT(LIST, VALUETYPE)

#define IMPLEMENT_LIST(LIST, VALUETYPE, INDEXTYPE, FN_CPY_VAL, FN_CPY_IDX, FN_CMP_VAL, FN_CMP_IDX, FN_FREE_VAL, FN_FREE_IDX) \
IMPLEMENT_LIST_FN_NEW(LIST, VALUETYPE, INDEXTYPE, FN_CPY_VAL, FN_CPY_IDX, FN_CMP_VAL, FN_CMP_IDX, FN_FREE_VAL, FN_FREE_IDX); \
IMPLEMENT_LIST_FN_FREE(LIST); \
IMPLEMENT_LIST_FN_ADD_STRUCT(LIST, VALUETYPE, INDEXTYPE); \
IMPLEMENT_LIST_FN_REMOVE_STRUCT(LIST, INDEXTYPE); \
IMPLEMENT_LIST_FN_GET_STRUCT(LIST, INDEXTYPE); \
IMPLEMENT_LIST_FN_SEARCH_STRUCT(LIST, VALUETYPE) \

#endif // __LIST_H__