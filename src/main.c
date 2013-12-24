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
#include "parse_input.h"
#include "simulator.h"

#define DEFAULT_NUM_PLAYERS 4
#define DEFAULT_NUM_SIMULATIONS 1000000
#define MAX_IN_LEN 13

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
	int np = DEFAULT_NUM_PLAYERS;		/* number of players */
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

static void
get_std_in(char *buf, size_t buflen)
{
	memset(buf, 0, buflen);
	fgets(buf, buflen, stdin);
	
	/* Strip newline and replace with null-byte */
	while ( (*buf != '\n') && (*buf != '\0')) buf++;
	*buf = 0;
}

/*
 * Queries the user 
 */
static void
query_run_sim(int *runSim)
{
	char buf[3];
	char c;
	memset(buf, 0, 3);
	fprintf(stdout, "Run simulation now? (y/n) ");
	fflush(stdout);
	/* Eat newlines on stdin. */
	while (1) {
		c = fgetc(stdin);
		if (c != '\n') {
			buf[0] = c;
			break;
		}
	}
	
	fgets(buf+1, 3, stdin);
	if ( (*buf == 'n') || (*buf == 'N') ) *runSim = 0;
	else *runSim = 1;
}

int
main(int argc, char *argv[])
{
	struct Args args;
	char playerHandStr[5], boardCardsStr[12], cardStr[3];
	int runSim = 0;

	/* Get cmd line args (if any) */	
	parseArgs(argc, argv, &args);

	/* Memset boardCardsStr in case we run pre-flop simulation. */
	memset(boardCardsStr, 0, sizeof(boardCardsStr));

	/* Get master player's hand. -----------------------------------------*/
	fprintf(stdout, "Please input your hand: ");
	get_std_in(playerHandStr, sizeof(playerHandStr));
	parse_input_string(playerHandStr, strlen(playerHandStr), PLAYER_HAND);
	query_run_sim(&runSim);
	if (runSim) {
		run_simulations(args.np, args.ns, playerHandStr, boardCardsStr);
	}

	/* Get flop. ---------------------------------------------------------*/
	fprintf(stdout, "\nPlease enter the flop cards: ");
	get_std_in(boardCardsStr, sizeof(boardCardsStr));
	parse_input_string(boardCardsStr, strlen(boardCardsStr), FLOP);
	query_run_sim(&runSim);
	if (runSim) {
		printf("should run simulations\n");
		run_simulations(args.np, args.ns, playerHandStr, boardCardsStr);
	}

	return 0;

	/* Get turn card. */

	/* Run post-turn simulations. */
	if (runSim) {
		run_simulations(args.np, args.ns, playerHandStr, boardCardsStr);
	}

	/* Get river card. */

	/* Run post-river simulations. */
	
	return 0;
}
