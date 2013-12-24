/*
 * Module which accumlates simulation stats and calculates final values to
 * be reported.
 */
#ifndef STATS_H
#define STATS_H

#include <err.h>
#include <stdio.h>
#include <stdlib.h>


/*
 * Ideas:
 *	- in order to determine the correct number of simulations to run,
 * 	  maybe we run some default number (e.g., 1000), record the results,
 * 	  then run another 500 or so and determine if the results changed
 *	  "significantly" (i.e., within some epsilon tolerance value that we'll
 *	  set by hand for now).  If the results changed by more than this
 * 	  epsilon, then we re run another 500 simulations and make the same 
 * 	  check.  If not, then we conclude we're "close enough" to the soln
 * 	  we want and stop running simulations.  (this is assuming our results
 * 	  will actually converge to any given value... is there a way to
 * 	  analytically prove this?)
 */
typedef struct
{
	/* Array keeping track of number of times the given hand was of a 
	 * certain relative strength (e.g., ranks[i] = number of times the 
	 * given hand was the (i+1)st best hand).
	 */
	int *ranks;

	/* stores results in a manner analogous to the ranks array */
	double *results;
 
	int nPlayers;		/* number of players in the game */
	int nSimulations;	/* total number of simulations */

} StatsStruct;

StatsStruct *init_stats_struct(int);
void update_stats(StatsStruct *, int);
void calculate_results(StatsStruct *);
void print_stats_struct(StatsStruct *);
void free_stats_struct(StatsStruct *);


#endif

