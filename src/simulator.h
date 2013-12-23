/*
 * Module responsible for running Texas Holdem poker game simulations and 
 * reporting game statistics. 
 */
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <poker-eval/poker_defs.h>
#include <poker-eval/deck_std.h>
#include <poker-eval/enumerate.h>

#include "stats.h"

void run_simulations(int, int);

#endif
