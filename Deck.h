#ifndef __DECK_H__
#define __DECK_H__


typedef struct Deck Deck;

typedef enum Deck_Result {
	DECK_SUCCESS,
	DECK_UNINITIALIZED_ERROR,				/**< Uninitialized GAME error 	*/
	DECK_ALLOCATION_ERROR,				/**< realloc error on grow/shrink   */
	DECK_INDEX_OUT_OF_BOUNDS_ERROR,
	DECK_OVERFLOW_ERROR,
	DECK_UNDERFLOW_ERROR
} DeckResult;

/**  
 * @brief Create a new Deck object
 * @param[in] _packNum - how many packs of cards are in the deck
 * @return Deck * - on success / NULL on fail
 */
Deck* DeckCreate(size_t _packNum);

/**  
 * @brief Deallocate a previously allocated Deck 
 * @param[in] _deck - Round to be deallocated
 * @return void
 */
void DeckDestroy(Deck** _deck);

/**  
 * @brief Reorganize the cards in the Deck object
 * @param[in] _deck - Deck object to be shuffled
 * @return void
 */
void DeckShuffle(Deck* _deck);

/**  
 * @brief Draw a card from Deck object
 * @param[in] _deck - Deck object to draw card from
 * @param[in] _index - the index of the card to draw
 * @param[in] _value - pointer to store the drawed card
 * @return DECK_SUCCESS or error code
 *
 * @warning DECK_UNINITIALIZED_ERROR - _deck or _value is NULL
 * @warning DECK_INDEX_OUT_OF_BOUNDS_ERROR - _index is out of range
 */
DeckResult DeckGet(const Deck* _deck, const size_t _index, void **_value);

/**  
 * @brief Prints a Deck object
 * @param[in] _deck - Deck object to print
 * @return void
 */
void PrintDeck(Deck *_deck);
#endif /* __DECK_H__ */
