#include <stdio.h>/*for size_t*/
#include <stdlib.h>/*for malloc*/
#include <string.h>/*for strcpy*/
#include "Player.h"
#include "Vector.h"
#include "Card.h"
#include "HeartsUI.h"
#include "GenericVectorBubbleSort.h"

struct Player
{
	char m_name[128];
	PlayerType m_type;
	size_t m_cardNumber;/*number of cards that player begins with*/
	Vector* m_cards;
};

/*Assisting Functions:*/
static PlayerResult CheckParams(size_t _cardNum);
static PlayerResult CheckIndex(const Player *_player, const size_t _index);
static int PlayerPrintElement(void* _element, size_t _index, void* _context);
static void PlayerDestroyElement(void* _item);
static int PlayerSuitElement(void* _element, size_t _index, void* _context);
static int PlayerFindElement(void* _element, size_t _index, void* _context);
VectorResult SortCriteria(const void *_first, const void *_second);

Player* PlayerCreate(PlayerType _type, size_t _cardNum, char* _name)
{
	Player* player = NULL;
	if(CheckParams(_cardNum) == PLAYER_WRONG_INPUT || _name == NULL)
	{
		return NULL;
	}
	if((player = (Player*)malloc(sizeof(Player))) == NULL)
	{
		return NULL;
	}
	player->m_cards = VectorCreate(_cardNum, 0);
	if(player->m_cards == NULL)
	{
		free(player);
		return NULL;
	}
	player->m_cardNumber = _cardNum;
	player->m_type = _type;
	strcpy(player->m_name, _name);
	return player;
}

void PlayerDestroy(Player** _player)
{
	if(_player != NULL && *_player != NULL)
	{
		VectorDestroy(&(*_player)->m_cards, PlayerDestroyElement);
		free(*_player);
		*_player = NULL;
	}
}

PlayerResult PlayerAppend(Player* _player, void* _value)
{
	if(_player == NULL && _value == NULL)
	{
		return PLAYER_UNINITIALIZED_ERROR;
	}
	VectorAppend(_player->m_cards, _value);
	return PLAYER_SUCCESS;
}

PlayerResult PlayerRemove(Player* _player, void** _value)
{
	if(_player == NULL && _value == NULL)
	{
		return PLAYER_UNINITIALIZED_ERROR;
	}
	VectorRemove(_player->m_cards, _value);
	return PLAYER_SUCCESS;
}

PlayerResult PlayerGet(Player* _player, const size_t _index, void** _value)
{
	if(_player == NULL && _value == NULL)
	{
		return PLAYER_UNINITIALIZED_ERROR;
	}
	if(CheckIndex(_player, _index) == PLAYER_INDEX_OUT_OF_BOUNDS_ERROR)
	{
		return PLAYER_INDEX_OUT_OF_BOUNDS_ERROR;
	}
	VectorGet(_player->m_cards,_index, _value);
	return PLAYER_SUCCESS;
}

PlayerResult PlayerSet(Player* _player, const size_t _index, void* _value)
{
	if(_player == NULL && _value == NULL)
	{
		return PLAYER_UNINITIALIZED_ERROR;
	}
	if(CheckIndex(_player, _index) == PLAYER_INDEX_OUT_OF_BOUNDS_ERROR)
	{
		return PLAYER_INDEX_OUT_OF_BOUNDS_ERROR;
	}
	VectorSet(_player->m_cards,_index, _value);
	return PLAYER_SUCCESS;
}

size_t HandSize(const Player* _player)
{
	if(_player == NULL)
	{
		return 0;
	}
	return VectorSize(_player->m_cards);
}

void PrintPlayerCards(Player *_player)
{
	if(_player == NULL)
	{
		return;
	}
	PrintPlayerName(_player->m_name);
	VectorForEach(_player->m_cards, PlayerPrintElement, NULL);
	printf("\n");
}

void PrintPlayerCardsWithIndex(Player *_player)
{
	int i;
	if(_player == NULL)
	{
		return;
	}
	PrintPlayerName(_player->m_name);
	for(i = 1;i <= VectorSize(_player->m_cards);++i)
	{
		printf(" %-3d",i);
	}
	printf("\n");
	VectorForEach(_player->m_cards, PlayerPrintElement, NULL);
	printf("\n");
}

void SortHand(Player *_player)
{
	if(_player == NULL)
	{
		return;
	}
	GenSort(_player->m_cards, VectorSize(_player->m_cards), sizeof(Card), SortCriteria);
}

PlayerResult PlayerGetName(Player *_player, char* _name)
{
	if(_player == NULL && _name == NULL)
	{
		return PLAYER_UNINITIALIZED_ERROR;
	}
	strncpy(_name, _player->m_name, 120);
	return PLAYER_SUCCESS;
}

PlayerResult PlayerGiveCard(Player *_player, void* _context, Rules _Rules, Stratagy _Stratagy, size_t *_index)
{
	size_t *availableIndex, availableIndexSize;
	if(_player == NULL && _index == NULL)
	{
		return PLAYER_UNINITIALIZED_ERROR;
	}
	if((availableIndex = (size_t*)calloc(_player->m_cardNumber, sizeof(size_t))) == NULL)
	{
		return PLAYER_ALLOCATION_ERROR;
	}
	availableIndexSize = _Rules(_player, _context, availableIndex);
	*_index = _Stratagy(_player, _context, availableIndex, availableIndexSize);
	free(availableIndex);
	return PLAYER_SUCCESS;
}

PlayerResult PlayerSuitExist(Player *_player, Suit _suit)
{
	size_t index;
	if(_player == NULL)
	{
		return PLAYER_UNINITIALIZED_ERROR;
	}
	index = VectorForEach(_player->m_cards, PlayerSuitElement, &_suit);
	if(index < VectorSize(_player->m_cards))
	{
		return PLAYER_TRUE;
	}
	return PLAYER_FALSE;
}

PlayerResult PlayerFindCard(Player *_player, Card _card, size_t *_index)
{
	if(_player == NULL || _index == NULL)
	{
		return PLAYER_UNINITIALIZED_ERROR;
	}
	*_index = VectorForEach(_player->m_cards, PlayerFindElement, &_card);
	if(*_index < VectorSize(_player->m_cards))
	{
		return PLAYER_TRUE;
	}
	return PLAYER_FALSE;
}

PlayerType PlayerGetType(Player *_player)
{
	if(_player == NULL)
	{
		return PLAYER_UNINITIALIZED_ERROR;
	}
	return _player->m_type;
}

/*Assisting Functions:*/

static PlayerResult CheckParams(size_t _cardNum)
{
	if(_cardNum == 0)
	{
		return PLAYER_WRONG_INPUT;
	}
	return PLAYER_SUCCESS;
}

static PlayerResult CheckIndex(const Player *_player, const size_t _index)
{
	if (_index >= _player->m_cardNumber)
	{
		return PLAYER_INDEX_OUT_OF_BOUNDS_ERROR;
	}
	return PLAYER_SUCCESS;
}

static int PlayerPrintElement(void* _element, size_t _index, void* _context)
{
	Card* temp = (Card*)_element;
	PrintCard(temp);
	return 1;
}

static void PlayerDestroyElement(void* _item)
{
	return;
}

static int PlayerSuitElement(void* _element, size_t _index, void* _context)
{
	Card* temp = (Card*)_element;
	Suit tempSuit = *(Suit*)_context;
	if(temp->m_suit == tempSuit)
	{
		return 0;
	}
	return 1;
}

static int PlayerFindElement(void* _element, size_t _index, void* _context)
{
	Card *check = (Card*)_element, *compare = (Card*)_context;
	if(check->m_suit == compare->m_suit && check->m_rank == compare->m_rank)
	{
		return 0;
	}
	return 1;
}

VectorResult SortCriteria(const void *_first, const void *_second)
{
	Card *card1 = (Card*)_first, *card2 = (Card*)_second;
	if(card1->m_suit > card2->m_suit)
	{
		return VECTOR_TRUE;
	}
	if(card1->m_suit == card2->m_suit && card1->m_rank > card2->m_rank)
	{
		return VECTOR_TRUE;
	}
	return VECTOR_FALSE;
}
