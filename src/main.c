/*
 * Simple poker hand evaluator -- given a player's hand, the number of players
 * at the table, and the cards on the board (three to five) this program 
 * simulates a number of games to estimate the odds that the given player's 
 * hand is the first, second, third, etc. best hand which is determined by
 * the percentages of times the given hand was of a relative strength in the
 * simulated games. 
 */

#include <err.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "simulator.h"

#define DEFAULT_NUM_SIMULATIONS 1000000

static void
usage()
{
	fprintf(stderr, "Usage: ./bin/hand_evaluator -p nPlayers "
	    "-s nSimulations \n");
	exit(EXIT_FAILURE);
}

void parseArgs(int argc, char *argv[], struct Args *args)
{
	/* Optional args. */
	int np = 4;				/* number of players */
	int ns = DEFAULT_NUM_SIMULATIONS;	/* number of simulations */
	int option;

	while ((option = getopt(argc, argv, "p:s:")) != -1) {
		switch (option) {
		case 's':
			ns = atoi(optarg);
			break;
		case 'p':
			np = atoi(optarg);
			break;
		default:
			usage();
		}
	}
	args->np = np;
	args->ns = ns;
}

int
main(int argc, char *argv[])
{
	struct Args args;
	char playerHandStr[] = "2d7h";
	char boardCardsStr[] = "AhTs8h3s";
	
	parseArgs(argc, argv, &args);
	run_simulations(args.np, args.ns, playerHandStr, boardCardsStr);
	return 0;
}
