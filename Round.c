#include <stdio.h>/*for size_t*/
#include <stdlib.h>/*for malloc*/
#include "Round.h"
#include "Player.h"
#include "Deck.h"
#include "Card.h"
#include "HeartsUI.h"

struct Round
{
	Deck* m_deck;
};

typedef struct RoundInfo
{
	Card m_tableCards[PLAYER_NUM];
	Card m_cardsPlayed[52];
	size_t m_numOfCardsPlayed;
	size_t m_numPlayed;
	size_t m_firstPlayer;
	RoundResult m_heartsBroken;
	RoundResult m_spadeQueenPlayed;
	RoundResult m_playerSuitExist;
	Suit m_leadingSuit;
} RoundInfo;

/*Assisting Functions:*/
static RoundResult FindMin(int *_scores, size_t* _winnerIndex);
static void DealCards(Round* _round, Player** players);
static void TrickRun(Player** _players, int _firstPlayer, RoundInfo *_roundInfo);
static void DrawCard(Player* _player, Card** _card, RoundInfo *_roundInfo);
static int CalculateTrickScore(int _firstPlayer, int* _scores, Card* trickCards, int *loserIndex);
void Check26(int* _scores);
static void InitializeRoundInfo(RoundInfo *_roundInfo);
static RoundResult CheckSpadeQueen(Card *card);
static RoundResult CheckHeartsBroken(Card *card);
static void SwapCards(Player** _players, int _roundNum);
void DealSwapCards(Card *_swapCards[12], Player** _players, int _playerIndex);
static size_t StratagyRun(Player *_player, void *_roundInfo, size_t *_availableIndex, size_t _availableIndexSize, StratagyCriteria _ptrFunc);
static RoundResult SuitNotExistCriteria(const Card *_card1, const Card *_card2);
static RoundResult SuitExistCriteria(const Card *_card1, const Card *_card2);

Round* RoundCreate()
{
	Round* round = NULL;
	size_t packNum = 1;/*num of cards packs in the game*/	
	if((round = (Round*)malloc(sizeof(Round))) == NULL)
	{
		return NULL;
	}
	if((round->m_deck = DeckCreate(packNum)) == NULL)
	{
		free(round);
		return NULL;
	}
	return round;
}

void RoundDestroy(Round** _round)
{
	if(_round != NULL && *_round != NULL)
	{
		DeckDestroy(&(*_round)->m_deck);
		free(*_round);
		*_round = NULL;
	}
}

RoundResult RoundRun(Round* _round, int* _scores, Player** _players, int _roundNum)
{
	int i, loserIndex = 0, trickScore;
	size_t findCardIndex;
	Card startingCard = {CLUBS, TWO};
	RoundInfo *roundInfo = NULL;
	if(_round == NULL || _scores == NULL || _players == NULL)
	{
		return ROUND_UNINITIALIZED_ERROR;
	}
	if((roundInfo = (RoundInfo*)malloc(sizeof(RoundInfo))) == NULL)
	{
		return ROUND_ALLOCATION_ERROR;
	}
	InitializeRoundInfo(roundInfo);
	roundInfo->m_numOfCardsPlayed = 0;
	DeckShuffle(_round->m_deck);
	DealCards(_round, _players);
/*	for(i = 0;i < PLAYER_NUM;++i)*/
/*	{*/
/*		PrintPlayerCards(_players[i]);*/
/*	}*/
	if(_roundNum % PLAYER_NUM != 0)
	{
		SwapCards(_players, _roundNum);
	}
/*	for(i = 0;i < PLAYER_NUM;++i)*/
/*	{*/
/*		PrintPlayerCards(_players[i]);*/
/*	}*/
	for(i = 0;i < PLAYER_NUM;++i)
	{
		if(PlayerFindCard(_players[i], startingCard, &findCardIndex) == PLAYER_TRUE)
		{
			roundInfo->m_firstPlayer = i;
			break;
		}
	}
	for(i = 0;i < 13;++i)
	{
		TrickRun(_players, roundInfo->m_firstPlayer, roundInfo);
		trickScore = CalculateTrickScore(roundInfo->m_firstPlayer, _scores, roundInfo->m_tableCards, &loserIndex);
		EndTrickPrint(roundInfo->m_tableCards, _players[loserIndex], trickScore);
		roundInfo->m_firstPlayer = loserIndex;
		roundInfo->m_numPlayed = 0;
	}
	Check26(_scores);
	free(roundInfo);
	return ROUND_SUCCESS;
}

RoundResult HeartsCheckWin(int* _scores, size_t* _winnerIndex)
{
	int i;
	if(_scores == NULL || _winnerIndex == NULL)
	{
		return ROUND_UNINITIALIZED_ERROR;
	}
	for(i = 0;i < PLAYER_NUM;++i)
	{
		if(_scores[i] >= 100)
		{
			if(FindMin(_scores, _winnerIndex) == ROUND_TRUE)
			{
				return ROUND_TRUE;
			}
			break;
		}
	}
	return ROUND_FALSE;
}

/*Assisting Functions:*/

size_t RoundRules(Player *_player, void *_roundInfo, size_t *_availableIndex)
{
	int suitArr[NUM_OF_SUITS] = {0}, i;
	size_t handSize, j = 0;
	Card *card;
	handSize = HandSize(_player);
	if(((RoundInfo*)_roundInfo)->m_numOfCardsPlayed == 0)/*on first turn only play 2 of clubs*/
	{
		for(i = 0;i < handSize;++i)
		{
			PlayerGet(_player, i, (void**)&card);
			if(card->m_suit == CLUBS && card->m_rank == TWO)
			{
				_availableIndex[j] = i;
				++j;
				break;
			}
		}
		return j;
	}
	if(((RoundInfo*)_roundInfo)->m_numPlayed == 0)
	{
		for(i = 0;i < NUM_OF_SUITS;++i)
		{
			suitArr[i] = 1;
		}
		if(((RoundInfo*)_roundInfo)->m_heartsBroken == ROUND_FALSE)
		{
			suitArr[HEARTS] = 0;
		}
	}
	else if(PlayerSuitExist(_player, ((RoundInfo*)_roundInfo)->m_leadingSuit) == PLAYER_FALSE)
	{
		for(i = 0;i < NUM_OF_SUITS;++i)
		{
			suitArr[i] = 1;
		}
	}
	else
	{
		suitArr[((RoundInfo*)_roundInfo)->m_leadingSuit] = 1;
		((RoundInfo*)_roundInfo)->m_playerSuitExist = ROUND_TRUE;
	}
	for(i = 0;i < handSize;++i)
	{
		PlayerGet(_player, i, (void**)&card);
		if(suitArr[card->m_suit] == 1)
		{
			_availableIndex[j] = i;
			++j;
		}
	}
	if(j == 0)/*Hearts not broken and player has only hearts*/
	{
		for(i = 0;i < handSize;++i)
		{
			PlayerGet(_player, i, (void**)&card);
			if(card->m_suit == HEARTS)
			{
				_availableIndex[j] = i;
				++j;
			}
		}
	}
	return j;
}

size_t RoundStratagy(Player *_player, void *_roundInfo, size_t *_availableIndex, size_t _availableIndexSize)
{
	size_t wantedIndex;
	if(((RoundInfo*)_roundInfo)->m_playerSuitExist == ROUND_TRUE || ((RoundInfo*)_roundInfo)->m_numPlayed == 0)
	{
		wantedIndex = StratagyRun(_player, _roundInfo, _availableIndex, _availableIndexSize, SuitExistCriteria);
	}
	else
	{
		wantedIndex = StratagyRun(_player, _roundInfo, _availableIndex, _availableIndexSize, SuitNotExistCriteria);
	}
	return wantedIndex;
}

size_t SwapRules(Player *_player, void *_roundInfo, size_t *_availableIndex)
{
	size_t handSize, i;
	handSize = HandSize(_player);
	for(i = 0;i < handSize;++i)
	{
		_availableIndex[i] = i;
	}
	return i;
}

size_t SwapStratagy(Player *_player, void *_roundInfo, size_t *_availableIndex, size_t _availableIndexSize)
{
	size_t wantedIndex = 847, handSize, i;
	Rank highestRank = TWO;
	Card queenOfSpades = {SPADES, QUEEN}, *temp;
	RoundResult legalResponse = ROUND_FALSE;
	if(PlayerGetType(_player) == COMP)
	{
		if(PlayerFindCard(_player, queenOfSpades, &wantedIndex) == PLAYER_TRUE)
		{
			return wantedIndex;
		}
		handSize = HandSize(_player);
		for(i = 0;i < handSize;++i)
		{
			PlayerGet(_player, i, (void**)&temp);
			if(highestRank < temp->m_rank)
			{
				highestRank = temp->m_rank;
				wantedIndex = i;
			}
		}
	}
	else
	{
		wantedIndex = GetCardIndexForSwap(_player) - 1;
		while(legalResponse != ROUND_TRUE)
		{
			if(wantedIndex < _availableIndexSize)
			{
				legalResponse = ROUND_TRUE;
			}
			else
			{
				wantedIndex = GetCardIndexForSwapIlligal(_player) - 1;
			}
		}
	}
	return wantedIndex;
}

static RoundResult FindMin(int *_scores, size_t* _winnerIndex)
{
	int i, min, minIndex, minIsSame = 100;
	min = _scores[0];
	minIndex = 0;
	for(i = 1;i < PLAYER_NUM;++i)
	{
		if(_scores[i] == min)
		{
			minIsSame = _scores[i];
			continue;
		}
		if(_scores[i] < min)
		{
			min = _scores[i];
			minIndex = i;
		}
	}
	if(minIsSame == min)/*there are 2 players with the minimum score*/
	{
		*_winnerIndex = PLAYER_NUM;/*this indexis surley out of bounds*/
		return ROUND_FALSE;
	}
	*_winnerIndex = minIndex;
	return ROUND_TRUE;
}

static void DealCards(Round* _round, Player** _players)
{
	int playerCardNum = 13, i, j, deckIndex = 0;
	void *card;
	for(i = 0;i < playerCardNum;++i)
	{
		for(j = 0;j < PLAYER_NUM;++j)
		{
			DeckGet(_round->m_deck, deckIndex, &card);
			++deckIndex;
			PlayerAppend(_players[j], card);
		}
	}
	for(j = 0;j < PLAYER_NUM;++j)
	{
		SortHand(_players[j]);
	}
}

static void TrickRun(Player** _players, int _firstPlayer, RoundInfo *_roundInfo)
{
	int j;
	Card *card;
	for(j = 0;j < PLAYER_NUM;++j)
	{
		DrawCard(_players[(_firstPlayer + j) % PLAYER_NUM], &card, _roundInfo);
		_roundInfo->m_tableCards[(_firstPlayer + j) % PLAYER_NUM] = *card;
		_roundInfo->m_numPlayed = j + 1;
		_roundInfo->m_cardsPlayed[_roundInfo->m_numOfCardsPlayed] = *card;
		_roundInfo->m_numOfCardsPlayed += 1;
		if(j == 0)
		{
			_roundInfo->m_leadingSuit = card->m_suit;
		}
		if(_roundInfo->m_spadeQueenPlayed == ROUND_FALSE && CheckSpadeQueen(card) == ROUND_TRUE)
		{
			_roundInfo->m_spadeQueenPlayed = ROUND_TRUE;
		}
		if(_roundInfo->m_heartsBroken == ROUND_FALSE && CheckHeartsBroken(card) == ROUND_TRUE)
		{
			_roundInfo->m_heartsBroken = ROUND_TRUE;
		}
		_roundInfo->m_playerSuitExist = ROUND_FALSE;/*for the next player*/
	}
}

static void DrawCard(Player* _player, Card** _card, RoundInfo *_roundInfo)
{
	Card* lastCard;
	size_t lastIndex = HandSize(_player) - 1;
	size_t index;
	PlayerGiveCard(_player, _roundInfo, RoundRules, RoundStratagy, &index);
	PlayerGet(_player, index, (void**)_card);
	PlayerGet(_player, lastIndex, (void**)&lastCard);
	PlayerSet(_player, index, (void*)lastCard);
	PlayerRemove(_player, (void**)&lastCard);
	SortHand(_player);
}

static int CalculateTrickScore(int _firstPlayer, int* _scores, Card* trickCards, int *loserIndex)
{
	int i, trickScore = 0;
	Suit suit = trickCards[_firstPlayer].m_suit;
	Rank HighestRank = TWO;
	for(i = 0;i < PLAYER_NUM;++i)
	{
		if(trickCards[i].m_suit == suit && trickCards[i].m_rank >= HighestRank)
		{
			*loserIndex = i;
			HighestRank = trickCards[i].m_rank;
		}
		if(trickCards[i].m_suit == HEARTS)
		{
			trickScore += 1;
		}
		if(trickCards[i].m_suit == SPADES && trickCards[i].m_rank == QUEEN)
		{
			trickScore += 13;
		}
	}
	_scores[*loserIndex] += trickScore;
	return trickScore;
}

void Check26(int* _scores)
{
	int i, trickWinnerIndex = PLAYER_NUM;	
	for(i = 0;i < PLAYER_NUM;++i)
	{
		if(_scores[i] == 26)
		{
			trickWinnerIndex = i;
			break;
		}
	}
	if(trickWinnerIndex < PLAYER_NUM)
	{
		for(i = 0;i < PLAYER_NUM;++i)
		{
			if(i == trickWinnerIndex)
			{
				_scores[i] = 0;
			}
			else
			{
				_scores[i] = 26;
			}
		}
	}
}

static void InitializeRoundInfo(RoundInfo *_roundInfo)
{
	_roundInfo->m_numPlayed = 0;
	_roundInfo->m_heartsBroken = ROUND_FALSE;
	_roundInfo->m_spadeQueenPlayed = ROUND_FALSE;
	_roundInfo->m_playerSuitExist = ROUND_FALSE;
}

static RoundResult CheckSpadeQueen(Card *card)
{
	if(card->m_suit == SPADES && card->m_rank == QUEEN)
	{
		return ROUND_TRUE;
	}
	return ROUND_FALSE;
}

static RoundResult CheckHeartsBroken(Card *card)
{
	if(card->m_suit == HEARTS)
	{
		return ROUND_TRUE;
	}
	return ROUND_FALSE;
}

static void SwapCards(Player** _players, int _roundNum)
{
	Card *swapCards[12], *lastCard, *temp;
	int i, j, swapCardsIndex = 0, swapNum = 3;
	size_t index, lastIndex;
	for(i = 0;i < PLAYER_NUM;++i)
	{
		for(j = 0;j < swapNum;++j)
		{
			lastIndex = HandSize(_players[i]) - 1;
			PlayerGiveCard(_players[i], NULL, SwapRules, SwapStratagy, &index);
			PlayerGet(_players[i], index, (void**)&temp);
			swapCards[swapCardsIndex] = temp;
			++swapCardsIndex;
			PlayerGet(_players[i], lastIndex, (void**)&lastCard);
			PlayerSet(_players[i], index, (void*)lastCard);
			PlayerRemove(_players[i], (void**)&lastCard);
			SortHand(_players[i]);
		}
	}
	if(_roundNum % PLAYER_NUM == 1)
	{
		DealSwapCards(swapCards, _players, 1);
	}
	else if(_roundNum % PLAYER_NUM == 2)
	{
		DealSwapCards(swapCards, _players, 3);
	}
	else if(_roundNum % PLAYER_NUM == 3)
	{
		DealSwapCards(swapCards, _players, 2);
	}
}

void DealSwapCards(Card *_swapCards[12], Player** _players, int _playerIndex)
{
	int i;
	for(i = 0;i < 12;++i)
	{
		PlayerAppend(_players[_playerIndex], _swapCards[i]);
		if((i + 1) % 3 == 0)
		{
			SortHand(_players[_playerIndex]);
			_playerIndex = (_playerIndex + 1) % PLAYER_NUM;
		}
	}
}

static size_t StratagyRun(Player *_player, void *_roundInfo, size_t *_availableIndex, size_t _availableIndexSize, StratagyCriteria _ptrFunc)
{
	size_t i, wantedIndex = 456;
	Card *playerCard1, *playerCard2;
	RoundResult legalResponse = ROUND_FALSE;
	if(PlayerGetType(_player) == COMP)
	{
		wantedIndex = _availableIndex[0];
		PlayerGet(_player, _availableIndex[0], (void**)&playerCard1);
		for(i = 1;i < _availableIndexSize;++i)
		{
			PlayerGet(_player, _availableIndex[i],(void**)&playerCard2);
			if(_ptrFunc(playerCard1, playerCard2) == ROUND_TRUE)
			{
				playerCard1 = playerCard2;
				wantedIndex = _availableIndex[i];
			}
		}
	}
	else
	{
		wantedIndex = GetCardIndex(_player, ((RoundInfo*)_roundInfo)->m_numPlayed, ((RoundInfo*)_roundInfo)->m_tableCards, ((RoundInfo*)_roundInfo)->m_firstPlayer, ((RoundInfo*)_roundInfo)->m_leadingSuit) - 1;
		while(legalResponse != ROUND_TRUE)
		{
			for(i = 0;i < _availableIndexSize;++i)
			{
				if(wantedIndex == _availableIndex[i])
				{
					break;
				}
			}
			if(i < _availableIndexSize)
			{
				legalResponse = ROUND_TRUE;
			}
			else
			{
				wantedIndex = IllegalChioce(_player, ((RoundInfo*)_roundInfo)->m_numPlayed, ((RoundInfo*)_roundInfo)->m_tableCards, ((RoundInfo*)_roundInfo)->m_firstPlayer, ((RoundInfo*)_roundInfo)->m_leadingSuit) - 1;
			}
		}
	}
	return wantedIndex;
}

static RoundResult SuitNotExistCriteria(const Card *_card1, const Card *_card2)
{
	if(_card2->m_suit == SPADES && _card2->m_rank == QUEEN)
	{
		return ROUND_TRUE;
	}
	if(_card1->m_suit == SPADES && _card1->m_rank == QUEEN)
	{
		return ROUND_FALSE;
	}
	if(_card2->m_rank > _card1->m_rank)
	{
		if(_card2->m_suit == HEARTS || _card2->m_rank >= TEN)
		{
			return ROUND_TRUE;
		}
	}
	return ROUND_FALSE;
}

static RoundResult SuitExistCriteria(const Card *_card1, const Card *_card2)
{
	if(_card2->m_rank < _card1->m_rank)
	{
		return ROUND_TRUE;
	}
	return ROUND_FALSE;
}
