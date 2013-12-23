/*
 * Module responsible for running Texas Holdem poker game simulations and 
 * reporting game statistics. 
 */
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "stats.h"

void run_simulations(int, int);
void run_simulation(StatsStruct *);
void simulate_game();
void update_stats();

#endif
