
#include "parse_input.h"

static int is_valid_card_value(char);
static int is_valid_card_suite(char);

/*
 * Appends the given card to the string of board cards, assumes all 
 * input has been parsed and ensured to be correct.
 */
void append_card_to_string(char *boardCardStr, char *cardStr,
    InputType inputType)
{
	switch (inputType) {
	case TURN:
		boardCardStr[6] = *cardStr;
		boardCardStr[7] = *(cardStr + 1);
		boardCardStr[8] = 0;
		break;
	case RIVER:
		boardCardStr[8] = *cardStr;
		boardCardStr[9] = *(cardStr + 1);
		boardCardStr[10] = 0;
		break;
	default:
		fprintf(stderr, "Called with append_card_to_string() with "
		   "invalid input type.\n");
		exit(EXIT_FAILURE);
	}

}


/*
 * Given an input string, the length of the input string and the type of 
 * input to look for, returns 0 if the string is of an acceptable format and
 * 1 if it is not.
 */
int
parse_input_string(char *str, int len, InputType inputType)
{
	int expectedLen;
	
	if (!str) {
		fprintf(stderr, "Passed null string to parse_input_string.\n");
		exit(EXIT_FAILURE);
	}

	switch (inputType) {
	case PLAYER_HAND:
		expectedLen = 4;
		break;
	case FLOP:
		expectedLen = 6;
		break;
	case TURN:
		expectedLen = 2;
		break;
	case RIVER:
		expectedLen = 2;
		break;
	default:
		fprintf(stderr, "Invalid input: not of known type.\n");
		return 0; 
	}

	if (len != expectedLen) {
		fprintf(stderr, "Invalid input: not of expected length %d.\n",
		    expectedLen);
		return 0;
	}

	while (1) {
		
		if (!(is_valid_card_value(*str)) || 
		    !(is_valid_card_suite(*(str + 1)))) {
			fprintf(stderr, "Invalid input: invalid character(s) "
			    "among \"%c%c\".\n", *str, *(str+1)); 
			return 0;
		}

		/* If next char after current card != 0, then another card to
		 * validate. */
		if (*(str + 2) == 0) break;
		str += 2;
	}
	return 1;
}

static int is_valid_card_value(char c)
{
	switch (c) {
	case 'A':
	case 'K':
	case 'Q':
	case 'J':
	case 'T':
	case '9':
	case '8':
	case '7':
	case '6':
	case '5':
	case '4':
	case '3':
	case '2':
		return 1;
	default:
		return 0;
	}
}

static int is_valid_card_suite(char c)
{
	switch (c) {
	case 's':
	case 'd':
	case 'h':
	case 'c':
		return 1;
	default:
		return 0;
	}
}
