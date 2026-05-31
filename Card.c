#include <stdio.h>/*for size_t*/
#include <stdlib.h>/*for malloc*/
#include "Card.h"

Card* CardCreate(Rank _rank, Suit _suit)
{
	Card* cardPtr;
	if(_rank >= NUM_OF_RANKS || _rank < 0 || _suit >= NUM_OF_SUITS || _suit < 0)
	{
		return NULL;
	}
	if((cardPtr = (Card*)malloc(sizeof(Card))) == NULL)
	{
		return NULL;
	}
	cardPtr->m_suit = _suit;
	cardPtr->m_rank = _rank;
	return cardPtr;
}

void CardDestroy(Card** _card)
{
	if(_card != NULL && *_card != NULL)
	{
		free(*_card);
		*_card = NULL;
	}
}

/*Assisting Functions:*/

void DestroyCardElement(void* _item)
{
	free(((Card*)_item));
	return;
}
