#include <stdio.h>/*for size_t*/
#include <stdlib.h>/*for malloc*/
#include "Game.h"
#include "Round.h"
#include "Player.h"
#include "Card.h"
#include "HeartsUI.h"

struct Game
{
	size_t m_playersNum;
	Round* m_round;
	Player** m_players;
	int* m_scores;/*players accumulating score*/
};

/*Assisting Functions:*/
static GameResult CheckParams(size_t _playerNum, size_t _humanTypeNumber, size_t _playerCardNum);

Game* GameCreate(size_t _playersNum, size_t _humanTypeNumber, size_t _playerCardNum)
{
	Game *gamePtr = NULL;
	Player** players = NULL;
	Round* round = NULL;
	int* scores = NULL, i;
	char name[120];
	GameResult check;
	check = CheckParams(_playersNum, _humanTypeNumber, _playerCardNum);
	if (check != GAME_SUCCESS)
	{
		return NULL;
	}
	if((gamePtr = (Game*)malloc(sizeof(Game))) == NULL)
	{
		return NULL;
	}
	if((players = (Player**)malloc(_playersNum*sizeof(Player*))) == NULL)
	{
		free(players);
		free(gamePtr);
		return NULL;
	}
	if((scores = (int*)calloc(_playersNum, sizeof(int))) == NULL)
	{
		free(gamePtr);
		return NULL;
	}
	if((round = RoundCreate()) == NULL)
	{
		free(players);
		free(scores);
		free(gamePtr);
		return NULL;
	}
	for(i = 0;i < _playersNum;++i)
	{
		sprintf(name, "Player%d", i+1);
		if(i < _humanTypeNumber)
		{
			players[i] = PlayerCreate(HUMAN, _playerCardNum, name);/*check if create was succeful?*/
			continue;
		}
		players[i] = PlayerCreate(COMP, _playerCardNum, name);
	}
	gamePtr->m_playersNum = _playersNum;
	gamePtr->m_round = round;
	gamePtr->m_players = players;
	gamePtr->m_scores = scores;
	return gamePtr;
}

void GameDestroy(Game** _game, void (*_playerDestroy)(Player** _item), void (*_roundDestroy)(Round** _item))
{
	size_t playersNum;
	int i;
	if(_game != NULL && *_game != NULL)
	{
		playersNum = (*_game)->m_playersNum;
		for(i = 0;i < playersNum;++i)
		{
			PlayerDestroy(&((*_game)->m_players[i]));
		}
		free((*_game)->m_players);
		_roundDestroy(&((*_game)->m_round));
		free((*_game)->m_scores);
		free(*_game);
		*_game = NULL;
	}
}

GameResult GameRun(Game* _game, RoundResult (*_winCheck)(int* _scores, size_t* _winnerIndex), size_t* _winnerIndex)
{
	int* roundScore = NULL, i, roundNum = 1;
	char winnerName[120];
	if(_game == NULL)
	{
		return GAME_UNINITIALIZED_ERROR;
	}
	if(((roundScore = (int*)calloc(_game->m_playersNum, sizeof(int))) == NULL))
	{
		return GAME_ALLOCATION_ERROR;
	}
	while(_winCheck(_game->m_scores, _winnerIndex) != ROUND_TRUE)
	{
		RoundRun(_game->m_round, roundScore, _game->m_players, roundNum);
		for(i = 0;i < _game->m_playersNum;++i)
		{
			_game->m_scores[i] += roundScore[i];
			roundScore[i] = 0;
		}
		EndRoundPrint(_game->m_players, _game->m_scores, roundNum);
		roundNum += 1;
	}
	PlayerGetName(_game->m_players[*_winnerIndex], winnerName);
	PrintWinner(winnerName);
	free(roundScore);
	return GAME_SUCCESS;
}

/*Assisting Functions:*/
static GameResult CheckParams(size_t _playerNum, size_t _humanTypeNumber, size_t _playerCardNum)
{
	if(_playerNum == 0 || _humanTypeNumber > _playerNum || _playerCardNum == 0)
	{
		return GAME_WRONG_INPUT;
	}
	return GAME_SUCCESS;
}
