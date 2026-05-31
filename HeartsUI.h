#ifndef __HEARTSUI_H__
#define __HEARTSUI_H__
#include "Card.h"/*for Card struct*/
#include "Player.h"/*for Player struct*/

/**  
 * @brief Print start menu
 * @return void
 */
void StartMenu();

/**  
 * @brief Print message to choose the number of human players
 * @return The number of human players chosen
 */
size_t ChooseNumberOfHumanPlayers();

/**  
 * @brief Get a card from HUMAN Player
 * @param[in] _player - Player object
 * @param[in] _numPlayed - the number of players who placed a card
 * @param[in] _tableCards - The cards that are on the table
 * @param[in] _firstPlayerIndex - the first player of the trick
 * @param[in] _leadingSuit - the leading suit of the trick
 * @return index of the chosen card
 *
 * @warning returns -1 in case _player or _tableCards are NULL
 */
size_t GetCardIndex(Player *_player, int _numPlayed, Card *_tableCards, int _firstPlayerIndex, Suit _leadingSuit);

/**  
 * @brief Get a card from HUMAN Player in case he had a wrong choice before
 * @param[in] _player - Player object
 * @param[in] _numPlayed - the number of players who placed a card
 * @param[in] _tableCards - The cards that are on the table
 * @param[in] _firstPlayerIndex - the first player of the trick
 * @param[in] _leadingSuit - the leading suit of the trick
 * @return index of the chosen card
 *
 * @warning returns -1 in case _player or _tableCards are NULL
 */
size_t IllegalChioce(Player *_player, int _numPlayed, Card *_tableCards, int _firstPlayerIndex, Suit _leadingSuit);

/**  
 * @brief Get a card from HUMAN Player for swap in the begining on game
 * @param[in] _player - Player object
 * @return index of the chosen card
 *
 * @warning returns -1 in case _player or _tableCards are NULL
 */
size_t GetCardIndexForSwap(Player *_player);

/**  
 * @brief Get a card from HUMAN Player for swap in the begining on game in case he had a wrong choice before
 * @param[in] _player - Player object
 * @return index of the chosen card
 *
 * @warning returns -1 in case _player or _tableCards are NULL
 */
size_t GetCardIndexForSwapIlligal(Player *_player);

/**  
 * @brief Print the cards on the table
 * @param[in] _numPlayed - the number of players who played this trick
 * @param[in] _tableCards - array of table cards
 * @param[in] _firstPlayerIndex - the first player of the trick
 * @param[in] _leadingSuit - the leading suit of the trick
 * @return void
 */
void PrintTableCards(int _numPlayed, Card *_tableCards, int _firstPlayerIndex, Suit _leadingSuit);

/**  
 * @brief Print the loser of the trick and the points he scored in the current trick
 * @param[in] _tableCards - array of table cards
 * @param[in] _trickLoser - Player who lost in the trick
 * @param[in] _points - The amount of points he lost
 * @return void
 */
void EndTrickPrint(Card *_tableCards, Player* _trickLoser, int _points);

/**  
 * @brief Print the status at the end of the round (round number, score status)
 * @param[in] _players - Array of players playing
 * @param[in] _scores - accumulating score status
 * @param[in] _roundNum - Round number
 * @return void
 */
void EndRoundPrint(Player** _players, int *_scores, int _roundNum);

/**  
 * @brief Print Player name
 * @param[in] _name - players name
 * @return void
 */
void PrintPlayerName(char* _name);

/**  
 * @brief Print the Winners name
 * @param[in] _name - winners name
 * @return void
 */
void PrintWinner(char* _name);

/**  
 * @brief Print a card with Rank and Suit
 * @param[in] _card - Card to print
 * @return void
 */
void PrintCard(Card* _card);

/**  
 * @brief Print a Suit
 * @param[in] _suit - Suit to print
 * @return void
 */
void PrintSuit(Suit _suit);

/**  
 * @brief Print a Rank 
 * @param[in] _rank - Rank to print
 * @return void
 */
void PrintRank(Rank _rank);

#endif /* __HEARTSUI_H__ */
