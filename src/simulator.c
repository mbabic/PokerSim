/*
 * Module responsible for running Texas Holdem poker game simulations and 
 * reporting game statistics. 
 */

#include <stdio.h>
#include <stdlib.h>

#include "simulator.h"

/* Private function declarations. */
static void do_nothing();
static void run_simulation(int, StdDeck_CardMask, StdDeck_CardMask,
    StatsStruct *);
static StdDeck_CardMask str_to_poker_hand(char *);

/* Private function implementations */

/*
 * Runs the simulations.
 */
void
run_simulations(int nPlayers, int nSimulations, char *playerHandStr, 
    char *boardCardsStr)
{
	StatsStruct *stats = NULL;
	StdDeck_CardMask playerHand, boardCards;
	int i;


	/* Convert hand strings into hand bit masks (used by pokersource) */
	playerHand = str_to_poker_hand(playerHandStr); 
	boardCards = str_to_poker_hand(boardCardsStr); 

	/* Initialize stat-tracking structure.*/
	stats = init_stats_struct(nPlayers);

	/* Run simulations. */
	for (i = 0; i < nSimulations; i++) {
		DBPRINT(("Running simulation %d.\n", i));
		run_simulation(nPlayers, playerHand, boardCards, stats);
	}
	

}

/*
 * Given the player's hand and the cards on the board (i.e., the flop and
 * potentially the turn and river card), randomly deals out the rest of the
 * cards to the given number of players and determines the strength of 
 * the player's hand.
 */
static void
run_simulation(int nPlayers, StdDeck_CardMask playerHand, 
    StdDeck_CardMask boardCards, StatsStruct *stats)
{

	/* Cards that have already been dealt. */
	StdDeck_CardMask deadCards;

	/* Extra boards card that (may) need to be dealt. */	
	StdDeck_CardMask extraBoardCards; 

	int nBoardCards;	/* number of board cards */ 
	int i;

	/* Add playerHand and boardCards to deadCards */
	StdDeck_CardMask_OR(deadCards, playerHand, boardCards);

	/* Deal out rest of board cards (if needed) */
	/* In principle, it shouldn't matter if we deal board cards first
	 * or if we deal to the other players (even though in real games the
	 * other players have their cards dealt to them before the flop)
	 */
	nBoardCards = StdDeck_numCards(boardCards);
	if (nBoardCards == 3) {
	
		DECK_MONTECARLO_N_CARDS_D(StdDeck, extraBoardCards, deadCards,\
		    2, 1, do_nothing(););
		StdDeck_CardMask_OR(deadCards, deadCards, extraBoardCards);
	
	} else if (nBoardCards == 4) {

		DECK_MONTECARLO_N_CARDS_D(StdDeck, extraBoardCards, deadCards, \
		    2, 1, do_nothing(););
		StdDeck_CardMask_OR(deadCards, deadCards, extraBoardCards);

	}

	DBPRINT(("Dead cards after dealing to board: %s\n", 
	    StdDeck_maskString(deadCards)));

	/* Deal cards to the remaining players. */
	for(i = 0; i < nPlayers - 1; i++) {
			
	}

	 
}

/* 
 * Dummy function to be passed to pokersource enumeration macros expecting 
 * "inner loop" functions -- this is functionality we do not need.
 */
static void
do_nothing()
{

}

/*
 * Converts the given hand string into a pokersource bitmask.
 */
static StdDeck_CardMask 
str_to_poker_hand(char *handStr)
{
	StdDeck_CardMask card, hand;
	StdDeck_CardMask_RESET(hand);
	char cardStr[3];
	int cardIndex = -1;
	

	if (handStr && strlen(handStr)) {
		cardStr[0] = *handStr;
		cardStr[1] = *(handStr + 1);
		cardStr[2] = '\0';

		while (*cardStr) {
		
			/* Create card mask from string. */	
			StdDeck_stringToCard(cardStr, &cardIndex);
			card = StdDeck_MASK(cardIndex);
			StdDeck_CardMask_OR(hand, hand, card);
			
			/* Advance handStr pointer to next card in the string */
			/* First ensure next byte is not null-byte. */
			handStr += 1;
			if (!(*handStr)) break;

			/* 
			 * Else, we know another card is in the string, so 
			 * advance the pointer once more time and reset cardStr.
			 */
			handStr += 1;
			cardStr[0] = *handStr;
			cardStr[1] = *(handStr + 1);
			cardStr[2] = '\0';
		}
	}

	return hand;
}
