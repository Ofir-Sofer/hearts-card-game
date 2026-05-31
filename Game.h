#ifndef __GAME_H__
#define __GAME_H__
#include "Round.h"/*Player Type*/
#include "Player.h"/*Round Type*/

typedef struct Game Game;


typedef enum Game_Result {
	GAME_SUCCESS,
	GAME_UNINITIALIZED_ERROR,				/**< Uninitialized GAME error 	*/
	GAME_ALLOCATION_ERROR,				/**< realloc error on grow/shrink   */
	GAME_INDEX_OUT_OF_BOUNDS_ERROR,
	GAME_OVERFLOW_ERROR,
	GAME_UNDERFLOW_ERROR,
	GAME_WRONG_INPUT
} GameResult;

/**  
 * @brief Create a new Game object
 * @param[in] _playersNum - number of players playing the game
 * @param[in] _humanTypeNumber - the number of human player out of _playersNum
 * @param[in] _playerCardNum - the number of cards each player begins with
 * @return Game * - on success / NULL on fail
 *
 * @warning if _playersNum = 0 or _humanTypeNumber > _playersNum or _playerCardNum = 0
 */
Game* GameCreate(size_t _playersNum, size_t _humanTypeNumber, size_t _playerCardNum);

/**  
 * @brief Deallocate a previously allocated Game 
 * @param[in] _game - Game to be deallocated
 * @param[in] _playerDestroy - A function pointer to be used to destroy all Players elements in the Game
 *             or a null if no such destroy is required
 * @param[in] _roundDestroy - A function pointer to be used to destroy Round element in the Game
 *             or a null if no such destroy is required
 * @return void
 */
void GameDestroy(Game** _game, void (*_playerDestroy)(Player** _item), void (*_roundDestroy)(Round** _item));

/**  
 * @brief Run an allocated game
 * @param[in] _game - Game to run
 * @param[in] _winCheck - A function pointer to be used to check if the game ended, depends on the game
 * @param[in] _winnerIndex - the Player index in Game Players who won the game
 * @return GAME_SUCCESS or error code 
 *
 * @warning GAME_UNINITIALIZED_ERROR - in case one of the param is NULL
 * @warning GAME_ALLOCATION_ERROR - Allocation failed
 */
GameResult GameRun(Game* _game, RoundResult (*_winCheck)(int* _scores, size_t* _winnerIndex), size_t* _winnerIndex);

#endif /* __GAME_H__ */
