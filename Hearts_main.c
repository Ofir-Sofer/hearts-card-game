#include <stdio.h>/*for size_t*/
#include <stdlib.h>/*for malloc*/
#include "Game.h"
#include "Player.h"
#include "Round.h"
#include "Deck.h"
#include "HeartsUI.h"

int main(void)
{
	size_t response = 0;
	size_t winnerIndex;
	Game* game;
	StartMenu();
	response = ChooseNumberOfHumanPlayers();
	game = GameCreate(4, response, 13);
	GameRun(game, HeartsCheckWin, &winnerIndex);
	GameDestroy(&game, PlayerDestroy, RoundDestroy);
	return 0;
}
