/*
 * Simple poker hand evaluator -- given a player's hand, the number of players
 * at the table, and the cards on the board (three to five) this program 
 * simulates a number of games to estimate the odds that the given player's 
 * hand is the first, second, third, etc. best hand which is determined by
 * the percentages of times the given hand was of a relative strength in the
 * simulated games. 
 */

#include <stdio.h>

#include "simulator.h"

int
main(int argc, char *argv[])
{
	/* Get cmd line args:
	 * 	- number of players
	 * 	- number of simulations to run (defautls to 10,000)	
	 */
	int nPlayers = 0, nSimulations = 0;
	run_simulations(nPlayers, nSimulations);
	return 0;
}
