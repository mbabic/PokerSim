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

	ss->ranks = calloc(nPlayers, sizeof(int));
	if (!(ss->ranks)) err(1, "Failled to allocate memory for StatsStruct "
	    "member \"ranks\".");

	ss->results = calloc(nPlayers, sizeof(double));
	if (!(ss->results)) err(1, "Failed to allocate memory for StatsStrct "	
	    "member \"results\".");

	return ss;
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

