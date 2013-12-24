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
#define DEFAULT_NUM_THREADS 4
#define DEFAULT_NUM_SIMULATIONS 1000000

extern int nThreads;

static void
usage()
{
	fprintf(stderr, "Usage: ./bin/hand_evaluator -p nPlayers (must be >= 2)"
	    " -s nSimulations (must be >= 1) -t nThreads (must be >= 1)\n");
	exit(EXIT_FAILURE);
}

void parseArgs(int argc, char *argv[], struct Args *args)
{
	/* Optional args. */
	int np = DEFAULT_NUM_PLAYERS;		/* number of players */
	int ns = DEFAULT_NUM_SIMULATIONS;	/* number of simulations */
	int nt = DEFAULT_NUM_THREADS;		/* number of threads */
	int option;

	while ((option = getopt(argc, argv, "p:s:t:")) != -1) {
		switch (option) {
		case 's':
			ns = atoi(optarg);
			if (ns < 1) usage();
			break;
		case 'p':
			np = atoi(optarg);
			if (np < 2) usage();
			break;
		case 't':
			nt = atoi(optarg);
			if (nt < 1) usage();
			break;
		default:
			usage();
		}
	}
	args->np = np;
	args->ns = ns;
	args->nt = nt;
}

static void
get_std_in(char *buf, size_t buflen)
{
	char c;
	memset(buf, 0, buflen);
	
	/* Eat garbage chars on stdin. */
	while (1) {
		c = fgetc(stdin);
		if (c != '\n' && c != EOF && c != 0) {
			buf[0] = c;
			break;
		}
	}
	fgets(buf+1, buflen-1, stdin);

	if ( (*buf == 'q') || (*buf == 'Q') ) exit(EXIT_SUCCESS);
	
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
	
	fgets(buf+1, 2, stdin);
	if ( (*buf == 'n') || (*buf == 'N') ) *runSim = 0;
	else *runSim = 1;
}

/*
 * TODO: in the below we could factor out each round of simulation into
 * its own function.
 */
int
main(int argc, char *argv[])
{
	struct Args args;
	char playerHandStr[5], boardCardsStr[12], cardStr[3];
	int runSim = 0;

	/* Get cmd line args (if any) */	
	parseArgs(argc, argv, &args);

	/* Set number of threads to be used in simulation runs. */
	nThreads = args.nt;

	/* Memset boardCardsStr in case we run pre-flop simulation. */
	memset(boardCardsStr, 0, sizeof(boardCardsStr));

	/* Get master player's hand. -----------------------------------------*/
	fprintf(stdout, "Please input your hand: ");
	get_std_in(playerHandStr, sizeof(playerHandStr));
	while (!parse_input_string(playerHandStr, strlen(playerHandStr),
	    PLAYER_HAND)) {
		fprintf(stdout, "Please re-enter turn card (q to quit): ");
		get_std_in(playerHandStr, sizeof(playerHandStr));	
	}
	query_run_sim(&runSim);
	if (runSim) {
		run_simulations(args.np, args.ns, playerHandStr, boardCardsStr);
	}

	/* Get flop. ---------------------------------------------------------*/
	fprintf(stdout, "\nPlease enter the flop cards: ");
	get_std_in(boardCardsStr, sizeof(boardCardsStr));
	while (!parse_input_string(boardCardsStr, strlen(boardCardsStr), FLOP)){
		fprintf(stdout, "Please re-enter flop cards (q to quit): ");
		get_std_in(boardCardsStr, sizeof(boardCardsStr));	
	}
	query_run_sim(&runSim);
	if (runSim) {
		run_simulations(args.np, args.ns, playerHandStr, boardCardsStr);
	}

	/* Get turn card. ----------------------------------------------------*/
	fprintf(stdout, "\nPlease enter the turn card: ");
	get_std_in(cardStr, sizeof(cardStr));
	while (!parse_input_string(cardStr, strlen(cardStr), TURN)) {
		fprintf(stdout, "Please re-enter turn card (q to quit): ");
		get_std_in(cardStr, sizeof(cardStr));	
	}
	append_card_to_string(boardCardsStr, cardStr, TURN);
	query_run_sim(&runSim);
	if (runSim) {
		run_simulations(args.np, args.ns, playerHandStr, boardCardsStr);
	}

	/* Get river card. ---------------------------------------------------*/
	fprintf(stdout, "\nPlease enter the river card: ");
	get_std_in(cardStr, sizeof(cardStr));
	while (!parse_input_string(cardStr, strlen(cardStr), RIVER)) {
		fprintf(stdout, "Please re-enter river card (q to quit): ");
		get_std_in(cardStr, sizeof(cardStr));	
	}
	append_card_to_string(boardCardsStr, cardStr, RIVER);
	query_run_sim(&runSim);
	if (runSim) {
		run_simulations(args.np, args.ns, playerHandStr, boardCardsStr);
	}

	/* Done and done. */	
	return 0;
}
