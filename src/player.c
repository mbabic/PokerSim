/*
 * Module modelling a single player in the game -- the player has a hand and
 * a hand value.
 */

#include "player.h"

/*
 * Allocates memory for a new Player struct and returns pointer to mem location.
 */
Player *
init_player()
{
	Player *p = NULL;

	p = calloc(1, sizeof(Player));
	if (!p) err(1, "Failed to allocate memory for Player struct.");

	return p;
}

/*
 * Given pointers to a Player and a card mask, copies the value of the card
 * mask into the struct member "hand".
 */
void
set_player_hand(Player *p, StdDeck_CardMask hand)
{
	StdDeck_CardMask_RESET(p->hand);
	StdDeck_CardMask_OR(p->hand, p->hand, hand);
}

/*
 * Given pointers to a Player and a card mask representing the board cards,
 * set_player_hand_value() calculates the value of the Player's hand and set
 * the handValue struct member.
 */
void
set_player_hand_value(Player *p, StdDeck_CardMask boardCards)
{
	StdDeck_CardMask toEvaluate;
	StdDeck_CardMask_OR(toEvaluate, p->hand, boardCards);
	p->handValue = StdDeck_StdRules_EVAL_N(toEvaluate, 7);
}
