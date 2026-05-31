#ifndef __GENERICVECTORBUBBLESORT_H__
#define __GENERICVECTORBUBBLESORT_H__
#include "Vector.h"

/*description- This function compare between two values, gets generic pointers (needs casting before comparison)
input- _first- pointer to value 1 to copare
	_second- pointer to value 2 to copare
output- VECTOR_TRUE / VECTOR_FALSE
errors- none*/
typedef VectorResult(*Criteria)(const void *_first, const void *_second);/*function of comparison of specific type*/

/*description- This function sorts any kind of array not depending on its type
input- _arr- array to sort
	_size- number of items in the array
	_elementSize- the size of each element in the array
	_ptrFunc- comparison function 
output- VECTOR_SUCCESS
	VECTOR_UNINITIALIZED_ERROR
errors- VECTOR_UNINITIALIZED_ERROR- _arr or _ptrFunc ==  NULL
*/
VectorResult GenSort(Vector* _vector, size_t _size, size_t _elementSize, Criteria _ptrFunc);
#endif/*__GENERICVECTORBUBBLESORT_H__*/
