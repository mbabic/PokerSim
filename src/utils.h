/*
 * Defines utitlity macros.
 */
#ifdef DEBUG
#define DBPRINT(str) do { printf str ; fflush(stdout);} while (0)
#else
#define DBPRINT(str) do {} while (0)
#endif
