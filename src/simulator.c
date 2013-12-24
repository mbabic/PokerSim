/*
 * Module responsible for running Texas Holdem poker game simulations and 
 * reporting game statistics. 
 */

#include <stdio.h>
#include <stdlib.h>

#include "simulator.h"

/*
 * The number of threads to be created -- set in main.c as part of cmd line
 * argument parsing.
 */
int nThreads;

/* 
 * Private pieces of states for each simulation (i.e., the variables
 * which do not change between simulations.
 */
static int nPlayers, nSimulations;
static StdDeck_CardMask playerHand, boardCards;

/*
 * Global stats struct in which results of all simulations of all threads are
 * accumulated.
 */
static StatsStruct *globalStats;

/*
 * Mutex protecting access to globalStats struct.
 */
static pthread_mutex_t statsMutex = PTHREAD_MUTEX_INITIALIZER;

static void do_nothing();
static void run_simulation(struct drand48_data *, struct TrialResults *);
static void *run_simulation_batch(void *);
static StdDeck_CardMask str_to_poker_hand(char *);


/*
 * Sets state vars, launches simulation threads.
 */
void
run_simulations(int np, int ns, char *playerHandStr, char *boardCardsStr)
{
	pthread_t threads[nThreads];
	pthread_attr_t attr;
	struct timeval start, end;	
	void *status;
	int i, ret;

	GET_TIME(start);

	/* Set number of players and number of simulations. */
	nPlayers = np;
	nSimulations = ns;

	/* Convert hand strings into hand bit masks (used by pokersource) */
	playerHand = str_to_poker_hand(playerHandStr); 
	boardCards = str_to_poker_hand(boardCardsStr); 

	/* Make threads joinable (so that we know when all threads have 
	 * finished executing). */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_setconcurrency(nThreads);

	/* Init global stats struct. */
	globalStats = init_stats_struct(nPlayers);

	/* Launch simulation batch threads. */
	for (i = 0; i < nThreads; i++) {
		ret = pthread_create(&threads[i], NULL, run_simulation_batch,
		    &i);
		if (ret) {
			err(1, "pthread_create returned error code %d", ret);
		}
	}

	/* Wait for all threads to finish executing. */
	for (i = 0; i < nThreads; i++) {
		ret = pthread_join(threads[i], &status);
		if (ret) err(1, "pthread_join returned error code %d", ret);
	}

	/* Tally and print out results. */
	calculate_results(globalStats);
	print_stats_struct(globalStats);

	GET_TIME(end);
	PRINT_TIME(start, end);
	/* Clean up allocated memory. */
	free_stats_struct(globalStats);
}

/*
 * Method passed to pthread_create -- it is the routine that each thread will
 * run in order to runs its subset of the simulations.
 */
static void *
run_simulation_batch(void *threadArgs)
{
	struct drand48_data randBuffer;
	struct timeval time;
	struct TrialResults results;
	StatsStruct *stats = NULL;
	long int seed;
	int i, nIterations, trialResult, *threadId;

	threadId = (int *) threadArgs;

	GET_TIME(time);

	seed = *threadId * 11231 + time.tv_usec;
	
	/* Call srand48 with seed value to initialize buf data. */
	srand48_r(seed, &randBuffer);

	stats = init_stats_struct(nPlayers);

	nIterations = (nSimulations / nThreads) + 1;
	
	for(i = 0; i < nIterations; i++) {
		run_simulation(&randBuffer, &results);
		update_stats(stats, results.rank, results.win);	
	} 

	
	/* Grab mutex and update global stats structure. */
	pthread_mutex_lock(&statsMutex);
	merge_stats_structs(globalStats, stats);
	pthread_mutex_unlock(&statsMutex);

	free_stats_struct(stats);
	pthread_exit((void *) 0);
}

/*
 * Given the player's hand and the cards on the board (i.e., the flop and
 * potentially the turn and river card), randomly deals out the rest of the
 * cards to the given number of players and determines the strength of 
 * the player's hand.  
 *
 * Takes as an argument the buf which stores information for the prng and
 * a TrialResults struct in which to store the results of the trial.. 
 *
 * Returns an integer representing the rank of the 
 * master player's hand in the trial.
 */
static void
run_simulation(struct drand48_data *randBuffer, struct TrialResults *results)
{
	/* Cards that have already been dealt. */
	StdDeck_CardMask deadCards;

	/* The complete set of board cards after additional ones have been dealt
 	 * to the table (if needed). */
	StdDeck_CardMask completeBoardCards;
	StdDeck_CardMask_RESET(completeBoardCards); 

	/* Hand which is combination of some player's hand and the board cards
	 * -- used when evaluated a card's strength. */
	StdDeck_CardMask evaluationHand;
	StdDeck_CardMask_RESET(evaluationHand); 

	/* Cards to be dealt. */	
	StdDeck_CardMask toDeal; 

	double ties = 0.0;	/* the number of opposition players whose hand
				 * is of equal value to the master player's */
	int nBoardCards;	/* number of board cards */ 
	int masterHandValue;	/* value of the master player's hand */
	int oppositionHandValue;/* value of opposing player's hand */
	int rank;		/* rank of master player's hand in trial */
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
	MONTECARLO_N_CARDS_SAMPLE(toDeal, deadCards, 5 - nBoardCards,
	    randBuffer);
	StdDeck_CardMask_OR(deadCards, deadCards, toDeal);
	StdDeck_CardMask_OR(completeBoardCards, completeBoardCards,
	    toDeal);
	

	/* Calculate the master player's hand value. */
	StdDeck_CardMask_OR(evaluationHand, playerHand, completeBoardCards);
	masterHandValue = StdDeck_StdRules_EVAL_N(evaluationHand, 7);

	/*
	 * Strategy for calculating master player's hand's rank: assume it is
	 * the best and anytime while dealing to other players and calculating
	 * the value of their hands if we encounter a hand value greater than
	 * the master player's hand's value we increment rank.
	 * Run time: O(nPlayers) time (as each value is considered only once)
	 */
	rank = 1;

	/* Deal cards to the remaining players and calculate values. */
	for(i = 0; i < nPlayers - 1; i++) {

		/* Deal player i two random (legal) cards. */
		StdDeck_CardMask_RESET(toDeal);
		MONTECARLO_N_CARDS_SAMPLE(toDeal, deadCards, 2, randBuffer);

		/* Add dealt cards to deadCards. */	
		StdDeck_CardMask_OR(deadCards, deadCards, toDeal);

		/* Set evaluation hand to be dealt cards and all board cards */
		StdDeck_CardMask_RESET(evaluationHand);
		StdDeck_CardMask_OR(evaluationHand, toDeal, completeBoardCards);

		oppositionHandValue = 
		    StdDeck_StdRules_EVAL_N(evaluationHand, 7);

		if (oppositionHandValue > masterHandValue) rank += 1;
		else if (oppositionHandValue == masterHandValue){
			 ties += 1.0;
		} 
	}
	
	/* If rank == 1, we won the hand and should report 1.0/ties as win
	 * value */
	if (rank == 1) results->win = 1.0/(ties + 1.0);
	else results->win = 0.0; 
	results->rank = rank;
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
	
	if (!handStr) return hand;
	if (strlen(handStr)) {
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
