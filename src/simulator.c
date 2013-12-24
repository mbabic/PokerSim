/*
 * Module responsible for running Texas Holdem poker game simulations and 
 * reporting game statistics. 
 */

#include <stdio.h>
#include <stdlib.h>

#include "simulator.h"

/* Private pieces of states for each simulation (i.e., the variables
 * which do not change between simulations */
static int nPlayers;
static Player *players;
static StdDeck_CardMask playerHand, boardCards;
static StatsStruct *stats;

/* Private function declarations. */
static void do_nothing();
static void run_simulation();
static StdDeck_CardMask str_to_poker_hand(char *);

/* Private function implementations */

/*
 * Runs the simulations.
 */
void
run_simulations(int np, int ns, char *playerHandStr, char *boardCardsStr)
{
	int i;

	/* Set number of players. */
	nPlayers = np;

	/* Allocate memory for the array of Players */
	players = calloc(nPlayers, sizeof(Player));
	if (!players) err(1, "Failed to allocate memory for Player array.");

	/* Convert hand strings into hand bit masks (used by pokersource) */
	playerHand = str_to_poker_hand(playerHandStr); 
	boardCards = str_to_poker_hand(boardCardsStr); 

	/* Initialize stat-tracking structure.*/
	stats = init_stats_struct(nPlayers);

	/* Run simulations. */
	for (i = 0; i < ns; i++) {
		DBPRINT(("Running simulation %d of %d.\n", i+1, ns));
		run_simulation();
	}
	
}

/*
 * Given the player's hand and the cards on the board (i.e., the flop and
 * potentially the turn and river card), randomly deals out the rest of the
 * cards to the given number of players and determines the strength of 
 * the player's hand.
 */
static void
run_simulation()
{

	/* Cards that have already been dealt. */
	StdDeck_CardMask deadCards;

	/* The complete set of board cards after additional ones have been dealt
 	 * to the table (if needed). */
	StdDeck_CardMask completeBoardCards;
	StdDeck_CardMask_RESET(completeBoardCards); 

	/* Cards to be dealt. */	
	StdDeck_CardMask toDeal; 

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
	StdDeck_CardMask_OR(completeBoardCards, completeBoardCards, boardCards);

	DECK_MONTECARLO_N_CARDS_D(StdDeck, toDeal, deadCards,\
	    5 - nBoardCards, 1, do_nothing(););
	StdDeck_CardMask_OR(deadCards, deadCards, toDeal);
	StdDeck_CardMask_OR(completeBoardCards, completeBoardCards, 
	    toDeal);
	
	DBPRINT(("Dead cards after dealing to board: %s\n", 
	    StdDeck_maskString(deadCards)));

	/* Deal cards to the remaining players and calculate values. */
	for(i = 1; i <= nPlayers - 1; i++) {

		/* Deal player i two random (legal) cards. */
		DECK_MONTECARLO_N_CARDS_D(StdDeck, toDeal, deadCards, \
		    2, 1, do_nothing(););

		DBPRINT(("Cards dealt to player %d: %s\n", 
		    i, StdDeck_maskString(toDeal)));

		/* Add dealt cards to deadCards. */	
		StdDeck_CardMask_OR(deadCards, deadCards, toDeal);

		/* Update player struct. */
		set_player_hand(&(players[i]), toDeal);
		set_player_hand_value(&(players[i]), completeBoardCards);
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
