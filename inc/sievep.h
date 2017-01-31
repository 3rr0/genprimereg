#pragma     once
#include    <inttypes.h>

/*
    Finds n prime numbers in [0;limit] using eratothenes sieve. 
    The primes are saved to p.
    Returns the number of found primes.
    Conditions: limit > n > 0
*/
uint32_t    sievep   ( uint32_t* p, uint32_t n, uint64_t limit );
