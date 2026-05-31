#ifndef __CARD_H__
#define __CARD_H__

typedef enum Rank {
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	JACK,
	QUEEN,
	KING,
	ACE,
	NUM_OF_RANKS
} Rank;

typedef enum Suit {
	HEARTS,
	SPADES,
	DIAMONDS,
	CLUBS,
	NUM_OF_SUITS
} Suit;

typedef struct Card
{
	Suit m_suit;
	Rank m_rank;
}Card;

/**  
 * @brief Create a new Card object
 * @param[in] _rank - New Card rank
 * @param[in] _suit - New Card suit
 * @return Card * - on success / NULL on fail
 */
Card* CardCreate(Rank _rank, Suit _suit);

/**  
 * @brief Deallocate a previously allocated Card 
 * @param[in] _card - Card to be deallocated
 * @return void
 */
void CardDestroy(Card** _card);

/**  
 * @brief Deallocate a previously allocated Card - used a function pointer to other modules
 * @param[in] _item - Card to be deallocated
 * @return void
 */
void DestroyCardElement(void* _item);
#endif /* __CARD_H__ */
