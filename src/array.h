#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stdio.h>
#include <stdlib.h>

// Helpers functions
// Compare functions
int Int_cmp(int n1, int n2);
int Float_cmp(float n1, float n2);
int Double_cmp(double n1, double n2);
int Ptr_cmp(void * pt1, void * pt2);
// Copy functions
void Int_copy(int * dest, int * src);
void Float_copy(float * dest, float * src);
void Double_copy(double * dest, double * src);
void Ptr_copy(void ** dest, void ** src);

// Free functions
void Int_free(int val);
void Float_free(float val);
void Double_free(double val);

// =============
//  Definitions
// =============
#define NEW_ARRAY_ELEM(ElemTypename, Valuetype, Indextype) \
typedef struct _ ## ElemTypename \
{ \
	Valuetype value; \
	Indextype index; \
	struct _ ## ElemTypename * next; \
	struct _ ## ElemTypename * prev; \
} ElemTypename

#define NEW_ARRAY_TYPE(ARRAY, Valuetype, Indextype) \
NEW_ARRAY_ELEM(ARRAY ## _elem_t, Valuetype, Indextype); \
typedef struct ARRAY \
{ \
	ARRAY ## _elem_t * array; \
	ARRAY ## _elem_t * end; \
	int    size; \
	size_t elemSize; \
	void (*_copyValue)(Valuetype * dest, Valuetype * src); \
	void (*_copyIndex)(Indextype * dest, Indextype * src); \
	int (*_cmpValue)(Valuetype val1, Valuetype val2); \
	int (*_cmpIndex)(Indextype val1, Indextype val2); \
	void (*_freeValue)(Valuetype value); \
	void (*_freeIndex)(Indextype index); \
} ARRAY

#define ARRAY_FN_NEW(ARRAY) \
ARRAY * ARRAY ## _new()

#define ARRAY_FN_FREE(ARRAY) \
void ARRAY ## _free(ARRAY * array)

#define ARRAY_FN_ADD_STRUCT(ARRAY, Valuetype, Indextype) \
ARRAY ## _elem_t * ARRAY ## _add(ARRAY * array, Indextype index, Valuetype value)

#define ARRAY_FN_REMOVE_STRUCT(ARRAY, Indextype) \
ARRAY * ARRAY ## _remove(ARRAY * array, Indextype index)

#define ARRAY_FN_GET_STRUCT(ARRAY, Indextype) \
ARRAY ## _elem_t * ARRAY ## _get(ARRAY * array, Indextype index)

#define ARRAY_FN_SEARCH_STRUCT(ARRAY, Valuetype) \
ARRAY ## _elem_t * ARRAY ## _search(ARRAY * array, Valuetype search)

//#define ARRAY_FN_GET_PREVIOUS(ARRAY, Indextype) \
//ARRAY ## _elem_t ARRAY ## _find_previous(ARRAY * array, Indextype index)

// =================
//  Implementations
// =================
#define IMPLEMENT_ARRAY_FN_NEW(ARRAY, Valuetype, Indextype, FN_CPY_VAL, FN_CPY_IDX, FN_CMP_VAL, FN_CMP_IDX, FN_FREE_VAL, FN_FREE_IDX) \
ARRAY * ARRAY ## _new() \
{ \
	ARRAY * array = malloc(sizeof(ARRAY)); \
	array->size  = 0; \
	array->array = NULL; \
	array->end   = NULL; \
	array->elemSize = sizeof(ARRAY ## _elem_t); \
	array->_copyValue = FN_CPY_VAL; \
	array->_copyIndex = FN_CPY_IDX; \
	array->_cmpValue  = FN_CMP_VAL; \
	array->_cmpIndex  = FN_CMP_IDX; \
	array->_freeValue = FN_FREE_VAL; \
	array->_freeIndex = FN_FREE_IDX; \
	return array; \
}

#define IMPLEMENT_ARRAY_FN_FREE(ARRAY) \
void ARRAY ## _free(ARRAY * array) \
{ \
	ARRAY ## _elem_t * it; \
	if(array == NULL) return; \
	for(it = array->array ; it != NULL ; it = it->next) \
	{ \
		if(it->prev) free(it->prev); \
		array->_freeValue(it->value); \
		array->_freeIndex(it->index); \
	} \
	free(array); \
}

#define IMPLEMENT_ARRAY_FN_ADD_STRUCT(ARRAY, Valuetype, Indextype) \
ARRAY ## _elem_t * ARRAY ## _add(ARRAY * array, Indextype index, Valuetype value) \
{ \
	ARRAY ## _elem_t * elem = NULL; \
	/* Test if array is NULL */\
	if(array == NULL) \
		return NULL; \
	/* Test if index exists */\
	if(ARRAY ## _get(array, index) != NULL) \
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


#define IMPLEMENT_ARRAY_FN_REMOVE_STRUCT(ARRAY, Indextype) \
ARRAY * ARRAY ## _remove(ARRAY * array, Indextype index) \
{ \
	ARRAY ## _elem_t * elem = ARRAY ## _get(array, index); \
	if(elem != NULL) \
	{ \
		array->_freeValue(elem->value); \
		array->_freeIndex(elem->index); \
		elem->prev->next = elem->next; \
		elem->next->prev = elem->prev; \
		free(elem); \
	} \
	array->size--; \
	return array; \
}


#define IMPLEMENT_ARRAY_FN_GET_STRUCT(ARRAY, Indextype) \
ARRAY ## _elem_t * ARRAY ## _get(ARRAY * array, Indextype index) \
{ \
	ARRAY ## _elem_t * out = NULL; \
	ARRAY ## _elem_t * it = NULL; \
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


#define IMPLEMENT_ARRAY_FN_SEARCH_STRUCT(ARRAY, Valuetype) \
ARRAY ## _elem_t * ARRAY ## _search(ARRAY * array, Valuetype search) \
{ \
	ARRAY ## _elem_t * out = NULL; \
	ARRAY ## _elem_t * it = NULL; \
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
#define NEW_ARRAY_DEFINITION(ARRAY, VALUETYPE, INDEXTYPE) \
NEW_ARRAY_TYPE(ARRAY, VALUETYPE, INDEXTYPE); \
ARRAY_FN_NEW(ARRAY); \
ARRAY_FN_FREE(ARRAY); \
ARRAY_FN_ADD_STRUCT(ARRAY, VALUETYPE, INDEXTYPE); \
ARRAY_FN_REMOVE_STRUCT(ARRAY, INDEXTYPE); \
ARRAY_FN_GET_STRUCT(ARRAY, INDEXTYPE); \
ARRAY_FN_SEARCH_STRUCT(ARRAY, VALUETYPE)

#define IMPLEMENT_ARRAY(ARRAY, VALUETYPE, INDEXTYPE, FN_CPY_VAL, FN_CPY_IDX, FN_CMP_VAL, FN_CMP_IDX, FN_FREE_VAL, FN_FREE_IDX) \
IMPLEMENT_ARRAY_FN_NEW(ARRAY, VALUETYPE, INDEXTYPE, FN_CPY_VAL, FN_CPY_IDX, FN_CMP_VAL, FN_CMP_IDX, FN_FREE_VAL, FN_FREE_IDX); \
IMPLEMENT_ARRAY_FN_FREE(ARRAY); \
IMPLEMENT_ARRAY_FN_ADD_STRUCT(ARRAY, VALUETYPE, INDEXTYPE); \
IMPLEMENT_ARRAY_FN_REMOVE_STRUCT(ARRAY, INDEXTYPE); \
IMPLEMENT_ARRAY_FN_GET_STRUCT(ARRAY, INDEXTYPE); \
IMPLEMENT_ARRAY_FN_SEARCH_STRUCT(ARRAY, VALUETYPE) \

#endif // __ARRAY_H__