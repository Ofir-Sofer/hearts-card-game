#include <stdio.h>/*for size_t*/
#include <ctype.h>
#include "Card.h"/*for Card struct*/
#include "HeartsUI.h"
#include "Player.h"
#include "Round.h"

#define HEART_PRINT "\x1B[31m\xE2\x99\xA5\x1B[0m"
#define SPADE_PRINT "\x1B[30m\xE2\x99\xA0\x1B[0m"
#define DIAMOND_PRINT "\x1B[31m\xE2\x99\xA6\x1B[0m"
#define CLUB_PRINT "\x1B[30m\xE2\x99\xA3\x1B[0m"


void StartMenu()
{
	char response = '0';
	printf("Welcome To Hearts Game\n");
	while(response != 'y')
	{
		puts("Would you like to start the game?[Y/N]");
/*		scanf("%c", &response);*/
		response = getchar();
		while(getchar() != '\n');
		response = tolower(response);
	}
}

size_t ChooseNumberOfHumanPlayers()
{
	size_t response = 0;
	printf("Enter the number of human players: ");
	scanf("%lu", &response);
	printf("\n");
	while(getchar() != '\n');
	return response;
}

size_t GetCardIndex(Player *_player, int _numPlayed, Card *_tableCards, int _firstPlayerIndex, Suit _leadingSuit)
{
	size_t response = 0;
	if(_player == NULL || _tableCards == NULL)
	{
		return -1;
	}
	PrintPlayerCardsWithIndex(_player);
	PrintTableCards(_numPlayed, _tableCards, _firstPlayerIndex, _leadingSuit);
	printf("Choose a card\n");
	scanf("%lu", &response);
	while(getchar() != '\n');
	return response;
}

size_t IllegalChioce(Player *_player, int _numPlayed, Card *_tableCards, int _firstPlayerIndex, Suit _leadingSuit)
{
	size_t response = 0;
	if(_player == NULL || _tableCards == NULL)
	{
		return -1;
	}
	printf("Choose again\n");
	PrintPlayerCardsWithIndex(_player);
	PrintTableCards(_numPlayed, _tableCards, _firstPlayerIndex, _leadingSuit);
	scanf("%lu", &response);
	while(getchar() != '\n');
	return response;
}

size_t GetCardIndexForSwap(Player *_player)
{
	size_t response = 0;
	if(_player == NULL)
	{
		return -1;
	}
	PrintPlayerCardsWithIndex(_player);
	printf("Choose a card to switch\n");
	scanf("%lu", &response);
	while(getchar() != '\n');
	return response;
}

size_t GetCardIndexForSwapIlligal(Player *_player)
{
	size_t response = 0;
	if(_player == NULL)
	{
		return -1;
	}
	printf("Choose again\n");
	PrintPlayerCardsWithIndex(_player);
	scanf("%lu", &response);
	while(getchar() != '\n');
	return response;
}

void PrintTableCards(int _numPlayed, Card *_tableCards, int _firstPlayerIndex, Suit _leadingSuit)
{
	int i;
	if(_tableCards == NULL)
	{
		return;
	}
	if(_numPlayed == 0)
	{
		printf("Table Is Empty\n");
	}
	else
	{
		for(i = 0;i < _numPlayed;++i)
		{
			PrintCard(&_tableCards[(_firstPlayerIndex + i) % PLAYER_NUM]);
		}
		printf(" Leading Suit Is:");
		PrintSuit(_leadingSuit);
		printf("\n");
	}
}

void EndTrickPrint(Card *_tableCards, Player* _trickLoser, int _points)
{
	int i;
	char name[120];
	if(_tableCards == NULL || _trickLoser == NULL)
	{
		return;
	}
	for(i = 0;i < PLAYER_NUM;++i)
	{
		PrintCard(&_tableCards[i]);
	}
	printf("\n");
	PlayerGetName(_trickLoser, name);
	printf("%s recieved %d points\n\n", name, _points);
	return;
}

void EndRoundPrint(Player** _players, int *_scores, int _roundNum)
{
	int i;
	char name[120];
	if(_players == NULL || _scores == NULL)
	{
		return;
	}
	printf("End of round %d:\n", _roundNum);
	for(i = 0;i < PLAYER_NUM;++i)
	{
		PlayerGetName(_players[i], name);
		printf("%s has %d points\n", name, _scores[i]);
	}
	printf("\n");
	return;
}

void PrintPlayerName(char* _name)
{
	puts(_name);
}

void PrintWinner(char* _name)
{
	printf("The Winner Is: %s\nCONGRATULATION\n", _name);
}

void PrintCard(Card* _card)
{
	Suit suit = _card->m_suit;
	Rank rank = _card->m_rank;
	PrintRank(rank);
	PrintSuit(suit);
	printf(" ");
}

void PrintSuit(Suit _suit)
{
	switch (_suit)
		{
			case HEARTS:
				printf(HEART_PRINT);
				break;
		
			case SPADES:
				printf(SPADE_PRINT);
				break;
				
			case DIAMONDS:
				printf(DIAMOND_PRINT);
				break;
				
			case CLUBS:
				printf(CLUB_PRINT);
				break;
				
			default:
				break;
		}
}

void PrintRank(Rank _rank)
{
	switch (_rank)
		{
			case TWO:
				printf(" 2");
				break;
		
			case THREE:
				printf(" 3");
				break;
				
			case FOUR:
				printf(" 4");
				break;
				
			case FIVE:
				printf(" 5");
				break;
				
			case SIX:
				printf(" 6");
				break;
			
			case SEVEN:
				printf(" 7");
				break;
			
			case EIGHT:
				printf(" 8");
				break;
			
			case NINE:
				printf(" 9");
				break;

			case TEN:
				printf("10");
				break;
				
			case JACK:
				printf(" J");
				break;
				
			case QUEEN:
				printf(" Q");
				break;
				
			case KING:
				printf(" K");
				break;
				
			case ACE:
				printf(" A");
				break;
				
			default:
				break;
		}
}
