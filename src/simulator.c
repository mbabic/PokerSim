/*
 * Module responsible for running Texas Holdem poker game simulations and 
 * reporting game statistics. 
 */


#include "simulator.h"

static int nPlayers, nSimulations;

void
run_simulations(int np, int ns)
{
	nPlayers = np;
	nSimulations = ns;
}

void
run_simulation(StatsStruct *stats)
{
}
