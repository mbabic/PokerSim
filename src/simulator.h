/*
 * Module responsible for running Texas Holdem poker game simulations and 
 * reporting game statistics. 
 */
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <err.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <poker-eval/poker_defs.h>
#include <poker-eval/deck_std.h>
#include <poker-eval/enumerate.h>
#include <poker-eval/inlines/eval.h>
#include <sys/time.h>

#include "stats.h"
#include "utils.h"

/* Below is a custom-defined Monte Carlo sampling of the deck of cards
 * which more multi-threaded friendly prng (before implementation of the macro
 * below it was the case that increasing the number of threads actually
 * _slowed down_ system performance. */
#define MONTECARLO_N_CARDS_SAMPLE(cards_var, dead_cards, num_cards, seedp)\
do {\
	StdDeck_CardMask _used;\
	int _i, _c;\
	StdDeck_CardMask_RESET(cards_var);\
	for (_i = 0; _i < num_cards; _i++) {\
		do {\
			_c = rand_r(&seedp) % StdDeck_N_CARDS;\
		} while (StdDeck_CardMask_CARD_IS_SET(_used, _c));\
		StdDeck_CardMask_SET(cards_var, _c);\
		StdDeck_CardMask_SET(_used, _c);\
	}\
} while (0) 

void run_simulations(int, int, char *, char *);

#endif
