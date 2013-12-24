#include "stats.h"

/*
 * Given the number of players in the simulations and the (initial) number
 * of simulations to be run, initializes and returns a pointer to an appropriate
 * StatsStruct object.
 */
StatsStruct *
init_stats_struct(int nPlayers) 
{
	StatsStruct *ss = NULL;

	ss = calloc(1, sizeof(StatsStruct));
	if (!ss) err(1, "Failed to allocate memory for StatsStruct.");

	ss->nSimulations = 0;
	ss->equity = 0.0;
	ss->nPlayers = nPlayers;

	ss->ranks = calloc(nPlayers, sizeof(int));
	if (!(ss->ranks)) err(1, "Failled to allocate memory for StatsStruct "
	    "member \"ranks\".");

	ss->results = calloc(nPlayers, sizeof(double));
	if (!(ss->results)) err(1, "Failed to allocate memory for StatsStrct "	
	    "member \"results\".");

	ss->wins = calloc(nPlayers, sizeof(double));
	if (!(ss->wins)) err(1, "Failed to allocate memory for StatsStrct "	
	    "member \"wins\".");
	return ss;
}

/*
 * Updates the given stats struct given that the player's hand was of
 * the given rank in the last simulation and updates the stats struct to 
 * reflect that another simulation has been run.
 */
void
update_stats(StatsStruct *ss, int rank)
{
	ss->ranks[rank-1] += 1;
	ss->nSimulations++;
}

/*
 * Calculates the results of the simulation.
 */
void
calculate_results(StatsStruct *ss)
{
	int nSimulations, nPlayers, i;
	nPlayers = ss->nPlayers;
	nSimulations = ss->nSimulations;
	for(i = 0; i < nPlayers; i++) {
		ss->results[i] = ((double)ss->ranks[i]) / (double)nSimulations; 
	}
}

/*
 * Prints the results struct in a pleasing way to stdout.
 */
void
print_stats_struct(StatsStruct *ss)
{
	int i;
	for(i = 0; i < ss->nPlayers; i++) {
		fprintf(stdout, "Likelihood given hand is of rank %d:\t %f\n",
		    i+1, ss->results[i]);
	}
}

/*
 * Frees the memory associated with the given StatsStruct object.
 */
void
free_stats_struct(StatsStruct *ss)
{
	if (!ss) return;
	if (ss->ranks) free(ss->ranks);
	if (ss->results) free(ss->results);
	free(ss);
}

