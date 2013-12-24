/*
 * Module implementing functionality with regard to acting upon and validating
 * input provided by the user.
 */
#ifndef PARSE_INPUT_H
#define PARSE_INPUT_H

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
	PLAYER_HAND,
	FLOP,
	TURN,
	RIVER
} InputType;

int parse_input_string(char *, int, InputType);
void append_card_to_string(char *, char *, InputType);
#endif
