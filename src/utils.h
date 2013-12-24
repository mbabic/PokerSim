/*
 * Defines utitlity macros.
 */

#include <sys/time.h>

#ifdef DEBUG
#define DBPRINT(str) do { printf str ; fflush(stdout);} while (0)
#else
#define DBPRINT(str) do {} while (0)
#endif

#define GET_TIME(s) do { gettimeofday(&s, NULL);} while (0)

#define PRINT_TIME(start, end) \
do { \
	long int diff = (end.tv_sec - start.tv_sec) * 1000;\
	diff += (end.tv_usec - start.tv_usec) / 1000;\
	printf("Simulations took %ld ms to complete.\n", diff);\
} while (0) 
