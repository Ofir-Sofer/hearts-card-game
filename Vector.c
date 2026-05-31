#include <stdio.h>/*for size_t*/
#include <stdlib.h>/*for malloc*/
#include "Vector.h"
#define BLOCK_SIZE_FACTOR 2

struct Vector
{
	void** m_items;
	size_t m_originalSize; /* original allocated space for items */
	size_t m_size; /* actual allocated space for items*/
	size_t m_nItems; /* actual number of items */
	size_t m_blockSize; /* the chunk size to be allocated when no space*/
};

/*assisting functions:*/
static VectorResult CheckParams(size_t _initialCapacity, size_t _blockSize);
static VectorResult CheckStructPtr(const Vector* _vector);
static VectorResult CheckIncrease(Vector *_vector);
static VectorResult CheckDecrease(Vector *_vector);
static VectorResult CheckIndex(const Vector *_vector, size_t _index);

Vector* VectorCreate(size_t _initialCapacity, size_t _blockSize)
{
	Vector *vectorPtr;
	void** m_items;
	VectorResult check;
	check = CheckParams(_initialCapacity, _blockSize);
	if (check != VECTOR_SUCCESS)
	{
		return NULL;
	}
	if((vectorPtr = (Vector*)malloc(sizeof(Vector))) == NULL)
	{
		return NULL;
	}
	if((m_items = (void**)malloc(_initialCapacity*sizeof(void*))) == NULL)
	{
		free(vectorPtr);
		free(m_items);
		return NULL;
	}
	vectorPtr->m_items = m_items;
	vectorPtr->m_originalSize = _initialCapacity;
	vectorPtr->m_size = _initialCapacity;
	vectorPtr->m_nItems = 0;
	vectorPtr->m_blockSize = _blockSize;
	return vectorPtr;
}

void VectorDestroy(Vector** _vector, void (*_elementDestroy)(void* _item))
{
	size_t size, i;
/*	void *pValue;*/
	if(_vector != NULL && CheckStructPtr(*_vector) == VECTOR_SUCCESS)
	{
		if(_elementDestroy != NULL)
		{
			size = VectorSize(*_vector);
			for(i = 0;i < size;++i)
			{
/*				VectorGet(*_vector, i, &pValue);*/
				_elementDestroy((*_vector)->m_items[i]);
			}
		}
		free((*_vector)->m_items);
		free(*_vector);
		*_vector = NULL;
	}
}

VectorResult VectorAppend(Vector* _vector, void* _item)
{
	VectorResult result;
	if(CheckStructPtr(_vector) == VECTOR_UNINITIALIZED_ERROR)
	{
		return VECTOR_UNINITIALIZED_ERROR;
	}
	result = CheckIncrease(_vector);
	if (result != VECTOR_SUCCESS)
	{
		return result;
	}
	_vector->m_items[_vector->m_nItems] = _item;
	_vector->m_nItems += 1;	
	return result;
}

VectorResult VectorRemove(Vector* _vector, void** _pValue)
{
	if(CheckStructPtr(_vector) == VECTOR_UNINITIALIZED_ERROR || _pValue==NULL)
	{
		return VECTOR_UNINITIALIZED_ERROR;
	}
	if (_vector->m_nItems == 0)
	{
		return VECTOR_UNDERFLOW_ERROR;
	}
	*_pValue = _vector->m_items[_vector->m_nItems-1];
	_vector->m_nItems -= 1;
	return CheckDecrease(_vector);
}

VectorResult VectorGet(const Vector* _vector, const size_t _index, void** _pValue)
{
	if(CheckStructPtr(_vector) == VECTOR_UNINITIALIZED_ERROR || _pValue == NULL)
	{
		return VECTOR_UNINITIALIZED_ERROR;
	}
	if(CheckIndex(_vector, _index) == VECTOR_INDEX_OUT_OF_BOUNDS_ERROR)
	{
		return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
	}
	*_pValue = _vector->m_items[_index];
	return VECTOR_SUCCESS;	
}

VectorResult VectorSet(Vector* _vector, const size_t _index, void*  _value)
{
	if(CheckStructPtr(_vector) == VECTOR_UNINITIALIZED_ERROR || _value == NULL)
	{
		return VECTOR_UNINITIALIZED_ERROR;
	}
	if(CheckIndex(_vector, _index) == VECTOR_INDEX_OUT_OF_BOUNDS_ERROR)
	{
		return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
	}
	_vector->m_items[_index] = _value;
	return VECTOR_SUCCESS;	
}

size_t VectorSize(const Vector* _vector)
{
	if(CheckStructPtr(_vector) == VECTOR_UNINITIALIZED_ERROR)
	{
		return 0;
	}
	return _vector->m_nItems;
}

size_t VectorCapacity(const Vector* _vector)
{
	if(CheckStructPtr(_vector) == VECTOR_UNINITIALIZED_ERROR)
	{
		return 0;
	}
	return _vector->m_size;
}

size_t VectorForEach(const Vector* _vector, VectorElementAction _action, void* _context)
{
	size_t mNItems;
	register size_t i;
	if(CheckStructPtr(_vector) == VECTOR_UNINITIALIZED_ERROR || _action == NULL)
	{
		return 0;
	}
	mNItems = _vector->m_nItems;
	for(i = 0; i < mNItems; ++i)
	{
    	if(_action(_vector->m_items[i], i, _context) == 0)
 		{
 			break;	
		}
	}
	return i;
}

/*Assisting Functions*/
static VectorResult CheckParams(const size_t _initialCapacity, const size_t _blockSize)
{
	if(_initialCapacity == 0 && _blockSize == 0)
	{
		return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;/*PARAMS_ERROR;*/
	}
	return VECTOR_SUCCESS;
}

static VectorResult CheckStructPtr(const Vector* _vector)
{
	if (_vector == NULL || _vector->m_items == NULL)
	{
		return VECTOR_UNINITIALIZED_ERROR;
	}
	return VECTOR_SUCCESS;
}

static VectorResult CheckIncrease(Vector *_vector)
{
	void **tempArr = NULL;
	if (_vector->m_nItems == _vector->m_size)
	{
		if (_vector->m_blockSize == 0)
		{
			return VECTOR_OVERFLOW_ERROR;
		}
		tempArr = (void**)realloc(_vector->m_items, (_vector->m_blockSize+_vector->m_size)*sizeof(void*));
		if (tempArr == NULL)
		{
			return VECTOR_REALLOCATION_ERROR;
		}
		_vector->m_items = tempArr;
		_vector->m_size += _vector->m_blockSize;
	}
	return VECTOR_SUCCESS;
}

static VectorResult CheckDecrease(Vector *_vector)
{
	void **tempArr = NULL;
	if (_vector->m_blockSize != 0 && _vector->m_size > _vector->m_originalSize &&
	_vector->m_size - _vector->m_nItems >= BLOCK_SIZE_FACTOR*_vector->m_blockSize)
	{
		tempArr = (void**)realloc(_vector->m_items, (_vector->m_size - _vector->m_blockSize)*sizeof(void*));
		if (tempArr == NULL)
		{
			return VECTOR_SUCCESS;/*Not outputing realloc failed because the item can be deleted without realloc*/
		}
		_vector->m_items = tempArr;
		_vector->m_size -= _vector->m_blockSize;
	}
	return VECTOR_SUCCESS;
}

static VectorResult CheckIndex(const Vector *_vector, size_t _index)
{
	if (_index >= _vector->m_nItems)
	{
		return VECTOR_INDEX_OUT_OF_BOUNDS_ERROR;
	}
	return VECTOR_SUCCESS;
}

/*get functions:*/
size_t GetVectorOriginalSize(const Vector *_vector)
{
	return _vector->m_originalSize;
}

size_t GetVectorBlockSize(const Vector *_vector)
{
	return _vector->m_blockSize;
}
