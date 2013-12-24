/* * Module modelling a single player in the game -- the player has a hand and
 * a hand value.
 */
#ifndef PLAYER_H
#define PLAYER_H
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <poker-eval/inlines/eval.h>
#include <poker-eval/poker_defs.h>

#include "utils.h"

typedef struct {
	int handValue;
	StdDeck_CardMask hand;
} Player;

Player *init_player();
void set_player_hand(Player *, StdDeck_CardMask);
void set_player_hand_value(Player *, StdDeck_CardMask);
#endif
