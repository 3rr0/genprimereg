#pragma     once

#define     N_FUNCTIONS			32		// Number of Functions per generation
#define     N_PARENTS			6		// Number of parents to pick
#define     N_PRIMES			16		// Number of primes
#define     SIEVE_LIMIT         100
#define     N_PRINTDATA			256		// Number of loops til print of data
#define     PERC_MUTATION		2		// Chance in percent of a mutation of a function
#define     FUNCTION_START		0x10	// Number of first function
#define     FUNCTION_ONEARG		0x17	// Number of first function to take one argument
#define     FUNCTION_END		0x1f	// Number of last function
#define     MAX_RAND_DOUBLE		65536	// Maximum double to be picked as random number
#define     PERC_ADD_NEW		60		// Percentage of adding a new function on creation

#define     DEBUG                       // If defined, every generation results will be printed to stdout
#ifdef      DEBUG
    #undef      N_PRINTDATA
    #define     N_PRINTDATA 1
#endif  // DEBUG