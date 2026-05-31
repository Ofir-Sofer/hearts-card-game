#ifndef __ROUND_H__
#define __ROUND_H__
#include "Player.h"/*for Player struct*/
#define PLAYER_NUM 4

typedef struct Round Round;

typedef enum Round_Result {
	ROUND_SUCCESS,
	ROUND_UNINITIALIZED_ERROR,				/**< Uninitialized GAME error 	*/
	ROUND_ALLOCATION_ERROR,				/**< realloc error on grow/shrink   */
	ROUND_INDEX_OUT_OF_BOUNDS_ERROR,
	ROUND_OVERFLOW_ERROR,
	ROUND_UNDERFLOW_ERROR,
	ROUND_TRUE,
	ROUND_FALSE
} RoundResult;

typedef RoundResult(*StratagyCriteria)(const Card *_card1, const Card *_card2);

/**  
 * @brief Create a new Round object
 * @param[in] void
 * @return Round * - on success / NULL on fail
 */
Round* RoundCreate();

/**  
 * @brief Deallocate a previously allocated Round 
 * @param[in] _round - Round to be deallocated
 * @return void
 */
void RoundDestroy(Round** _round);

/**  
 * @brief Run an allocated Round
 * @param[in] _round - Round to run
 * @param[in] _scores - A pointer to store the round score of each player
 * @param[in] players - Array of players participating the Round
 * @param[in] _roundNum - The round number
 * @return ERR_OK or error code 
 *
 * @warning ERR_NOT_INITIALIZED - in case one of the param is NULL
 * @warning ERR_ALOCATION_FAILED - Allocation failed
 */
RoundResult RoundRun(Round* _round, int* _scores, Player** players, int _roundNum);

/**  
 * @brief Checks if game of hearts over according to players scores
 * @param[in] _scores - Players points
 * @param[in] _winnerIndex - Index to store the index of the winner
 * @return TRUE/FALSE or error code
 *
 * @warning ERR_NOT_INITIALIZED
 */
RoundResult HeartsCheckWin(int* _scores, size_t* _winnerIndex);
#endif /* __ROUND_H__ */
