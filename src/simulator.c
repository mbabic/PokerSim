/*
 * Module responsible for running Texas Holdem poker game simulations and 
 * reporting game statistics. 
 */

#include <stdio.h>
#include <stdlib.h>

#include "simulator.h"

/* Module-only state variables. */
static int nPlayers, nSimulations;

/* Private member methods. */
static void do_nothing();
static void run_simulation(int, StdDeck_CardMask, StdDeck_CardMask,
    StatsStruct *);


void
run_simulations(int np, int ns)
{
	nPlayers = np;
	nSimulations = ns;
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

	StdDeck_CardMask_OR(deadCards, playerHand, boardCards);

	/* Deal out rest of board cards (if needed) */
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


	/* Good enough for now. */


	
	 
}

/* 
 * Dummy function to be passed to pokersource enumeration macros expecting 
 * "inner loop" functions -- this is functionality we do not need.
 */
static void
do_nothing()
{

}
