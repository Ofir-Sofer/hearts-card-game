#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "GenericVectorBubbleSort.h"
#define CURRENT (VectorGet(_vector, j, &temp1))
#define NEXT (VectorGet(_vector, j + 1, &temp2))
#define SWAP\
	{\
	VectorSet(_vector, j, temp2);\
	VectorSet(_vector, j + 1, temp1);\
	}

VectorResult GenSort(Vector* _vector, size_t _size, size_t _elementSize, Criteria _ptrFunc)
{
	void *temp1, *temp2;
	size_t i, j;
	int sorted=0;
	if(_vector == NULL || _ptrFunc == NULL)
	{
		return VECTOR_UNINITIALIZED_ERROR;
	}
	for(i = 0;i < _size;++i)
	{
		if (sorted)
		{
			break;
		}
		sorted = 1;
		for(j = 0;j < _size - 1 -i;++j)
		{
			CURRENT;
			NEXT;
			if(_ptrFunc(temp1, temp2) == VECTOR_TRUE)
			{
				SWAP;
				sorted = 0;
			}
		}
	}
	return VECTOR_SUCCESS;
}
