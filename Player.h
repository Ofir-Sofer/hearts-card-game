#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "Card.h"

typedef enum Player_Type {
	COMP,
	HUMAN
} PlayerType;

typedef struct Player Player;

typedef enum Player_Result {
	PLAYER_SUCCESS,
	PLAYER_UNINITIALIZED_ERROR,				/**< Uninitialized PLAYER error 	*/
	PLAYER_ALLOCATION_ERROR,
	PLAYER_REALLOCATION_ERROR,				/**< realloc error on grow/shrink   */
	PLAYER_INDEX_OUT_OF_BOUNDS_ERROR,
	PLAYER_OVERFLOW_ERROR,
	PLAYER_UNDERFLOW_ERROR,
	PLAYER_WRONG_INPUT,
	PLAYER_TRUE,
	PLAYER_FALSE
} PlayerResult;

typedef size_t (*Rules)(Player *_player, void* _context, size_t *_availableIndex);
typedef size_t (*Stratagy)(Player *_player, void* _context, size_t *_availableIndex, size_t _availableIndexSize);

/**  
 * @brief Create a new Player object
 * @param[in] _type - type of player HUMAN or COMP
 * @param[in] _cardNum - the number of cards the player starts with
 * @param[in] _name - the name of the player
 * @return Player * - on success / NULL on fail
 *
 * @warning _cardNum have to be greater then 0
 */
Player* PlayerCreate(PlayerType _type, size_t _cardNum, char* _name);

/**  
 * @brief Deallocate a previously allocated Player 
 * @param[in] _player - Player to be deallocated
 * @return void
 */
void PlayerDestroy(Player** _player);

/**  
 * @brief Add a new card to player hand
 * @param[in] _player - the Player to add card to
 * @param[in] _value - the new Card
 * @return PLAYER_SUCCESS or error code 
 *
 * @warning PLAYER_UNINITIALIZED_ERROR - in case params are NULL
 */
PlayerResult PlayerAppend(Player* _player, void* _value);

/**  
 * @brief Remove a card from Player
 * @param[in] _player - the Player to remove card from
 * @param[in] _value - pointer to store the removed card
 * @return PLAYER_SUCCESS or error code 
 *
 * @warning PLAYER_UNINITIALIZED_ERROR - in case params are NULL
 */
PlayerResult PlayerRemove(Player* _player, void** _value);

/**  
 * @brief Get a card from player according to index
 * @param[in] _player - Player object to get the card from
 * @param[in] _index - The index of the card in Player
 * @param[in] _value - pointer to store the card
 * @return PLAYER_SUCCESS or error code 
 *
 * @warning PLAYER_UNINITIALIZED_ERROR - in case params are NULL
 * @warning PLAYER_INDEX_OUT_OF_BOUNDS_ERROR - in case _index is out of bounds
 */
PlayerResult PlayerGet(Player* _player, const size_t _index, void** _value);

/**  
 * @brief Set a card to player according to index
 * @param[in] _player - Player object to set the card to
 * @param[in] _index - The index of the card in Player
 * @param[in] _value - the card to set
 * @return PLAYER_SUCCESS or error code 
 *
 * @warning PLAYER_UNINITIALIZED_ERROR - in case params are NULL
 * @warning PLAYER_INDEX_OUT_OF_BOUNDS_ERROR - in case _index is out of bounds
 */
PlayerResult PlayerSet(Player* _player, const size_t _index, void*  _value);

/**  
 * @brief Get the number of cards in player
 * @param[in] _player - Player object to get the number of cards
 * @return The number of cards in _player hand
 *
 * @warning if _player is NULL 0 is returned
 */
size_t HandSize(const Player* _player);

/**  
 * @brief Print the cards in players hand
 * @param[in] _player - Player object
 * @return void
 */
void PrintPlayerCards(Player *_player);

/**  
 * @brief Print the cards in players hand with index (starts from 1 to the total number of cards) above
 * @param[in] _player - Player object
 * @return void
 */
void PrintPlayerCardsWithIndex(Player *_player);

/**  
 * @brief Sort the cards in players hand according to Suit and Rank (Suit is defined in the Card module)
 * @param[in] _player - Player object
 * @return void
 */
void SortHand(Player *_player);

/**  
 * @brief Get Player name
 * @param[in] _player - Player object
 * @param[in] _name - pointer to store name
 * @return PLAYER_SUCCESS  or error code 
 *
 * @warning PLAYER_UNINITIALIZED_ERROR in casr _player is NULL
 */
PlayerResult PlayerGetName(Player *_player, char* _name);

/**  
 * @brief This function returns an index of a lligle card to draw from player
 * @param[in] _player - Player object
 * @param[in] _context - can be any variable to help the decision
 * @param[in] _Rules - Function pointer that define which cards in player are legal
 * @param[in] _Stratagy - Function pointer that chooses the best card out of the possible cards
 * @param[in] _index - Store the index of the appropriate card
 * @return PLAYER_SUCCESS or error code
 *
 * @warning PLAYER_UNINITIALIZED_ERROR in case _player or _index are NULL
 * @warning PLAYER_ALLOCATION_ERROR - Allocation failed
 */
PlayerResult PlayerGiveCard(Player *_player, void* _context, Rules _Rules, Stratagy _Stratagy, size_t *_index);

/**  
 * @brief Checks if the player has at least one card of the suit
 * @param[in] _player - Player object
 * @param[in] _suit - Suit to check
 * @return PLAYER_TRUE/ PLAYER_FALSE or error code
 *
 * @warning PLAYER_UNINITIALIZED_ERROR if _player is NULL
 */
PlayerResult PlayerSuitExist(Player *_player, Suit _suit);

/**  
 * @brief Checks if the player has a specific card
 * @param[in] _player - Player object
 * @param[in] _card - Card to check
 * @param[in] _index - Store the index of the appropriate card
 * @return PLAYER_TRUE/ PLAYER_FALSE or error code
 *
 * @warning PLAYER_UNINITIALIZED_ERROR if _player or _index is NULL
 */
PlayerResult PlayerFindCard(Player *_player, Card _card, size_t *_index);

/**  
 * @brief Get Player type (HUMAN/COMP)
 * @param[in] _player - Player object
 * @return Player type  or error code
 *
 * @warning PLAYER_UNINITIALIZED_ERROR if _player is NULL
 */
PlayerType PlayerGetType(Player *_player);
#endif /* __PLAYER_H__ */
