#include <stdio.h>/*for size_t*/
#include <stdlib.h>/*for malloc*/
#include <time.h>/*for srand*/
#include "Deck.h"
#include "Card.h"
#include "Vector.h"
#define PACK_SIZE 52/*num of cards in pack*/


struct Deck
{
	Vector* m_deckCards;
};

/*Assisting Functions:*/
static DeckResult CheckIndex(const Deck* _deck, const size_t _index);
int DeckPrintElement(void* _element, size_t _index, void* _context);
void PrintDeck(Deck *_deck);
/*void DestroyDeckElement(void* _item);*/

Deck* DeckCreate(size_t _packNum)
{
	int i, val, numOfCards;
	Deck* deck = NULL;
	Card* newCard;
	Rank rank;
	Suit suit;
	if((deck = (Deck*)malloc(sizeof(Deck))) == NULL)
	{
		return NULL;
	}
	if((deck->m_deckCards = VectorCreate(_packNum * PACK_SIZE, 0)) == NULL)
	{
		free(deck);
		return NULL;
	}
	numOfCards = _packNum * PACK_SIZE;
	for(i = 0;i < numOfCards;++i)
	{
		val = i % PACK_SIZE;/*make sure we only insert values that correspond to cards rank*/
		rank = val % NUM_OF_RANKS;
		suit = val / NUM_OF_RANKS;
		newCard = CardCreate(rank, suit);
		VectorAppend(deck->m_deckCards, newCard);
	}
	return deck;
}

void DeckDestroy(Deck** _deck)
{
	if(_deck != NULL && *_deck != NULL)
	{
		VectorDestroy(&(*_deck)->m_deckCards, DestroyCardElement);
		free(*_deck);
		*_deck = NULL;
	}
}

void DeckShuffle(Deck* _deck)/*give values to deck vector and shuffle the values*/
{
	int i, randInd;
	size_t numOfCards;
	void *temp1, *temp2;
	if(_deck->m_deckCards != NULL)
	{
		srand(time(NULL));
		numOfCards = VectorCapacity(_deck->m_deckCards);
		for(i = 0;i < numOfCards;++i)
		{
			VectorGet(_deck->m_deckCards, i, &temp1);
			randInd = rand() % numOfCards;
			VectorGet(_deck->m_deckCards, randInd, &temp2);
			VectorSet(_deck->m_deckCards, i, temp2);
			VectorSet(_deck->m_deckCards, randInd, temp1);
		}
	}
}

DeckResult DeckGet(const Deck* _deck, const size_t _index, void **_value)
{
	if(_deck == NULL && _value == NULL)
	{
		return DECK_UNINITIALIZED_ERROR;
	}
	if(CheckIndex(_deck, _index) == DECK_INDEX_OUT_OF_BOUNDS_ERROR)
	{
		return DECK_INDEX_OUT_OF_BOUNDS_ERROR;
	}
	VectorGet(_deck->m_deckCards, _index, _value);
	return DECK_SUCCESS;
}

/*Assisting Functions:*/
static DeckResult CheckIndex(const Deck* _deck, const size_t _index)
{
	if (_index >= VectorCapacity(_deck->m_deckCards))
	{
		return DECK_INDEX_OUT_OF_BOUNDS_ERROR;
	}
	return DECK_SUCCESS;
}

int DeckPrintElement(void* _element, size_t _index, void* _context)
{
	Card* temp = (Card*)_element;
	printf("%d %d ", temp->m_suit, temp->m_rank);
	printf("\n");
	return 1;
}

void PrintDeck(Deck *_deck)
{
	VectorForEach(_deck->m_deckCards, DeckPrintElement, NULL);
	printf("\n");
}
