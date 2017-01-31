/*
    Written 13.2.16 by SimPB

    Math.h needs to be linked with the flag -lm while compiling.
	Uses  2*limit / (limit - 2)  times less RAM than the version with even numbers:

	*
	*	Ges.:  k: Proportionalitätsfaktor
	*
	*		RAM_withEven = k * RAM_withoutEven
	*	<=> limit 		 = k * ( limit/2 - 1)
	*					 = k*limit / 2  - k
	*					 = k*limit / 2  - 2k/2														// Auf Hauptnenner bringen
	*					 = (limit-2)k / 2
	*	<=>	2*limit		 = (limit-2)k
	*	<=>	k			 = 2*limit / (limit-2)
	*


	- bereits gestrichene vielfache nicht mehr streichen. Wie?
		- würde mehr Aufwand als Zeitgewinn bringen
*/

#include    <sievep.h>
#include    <inttypes.h>
#include    <stdbool.h>
#include    <stdlib.h>
#include    <string.h>
#include    <math.h>


/// Check for prime.dat == NULL
uint32_t sievep( uint32_t* p, uint32_t N, uint64_t limit ) {
    p[0] = 2;                                                                                       // Needed because all even numbers are deprecated
    if( N == 1 ) return 1;
    
    uint32_t primeLen = ceil( limit / 2 ) - 1;
    bool*    prime    = (bool*)malloc( sizeof(bool) * primeLen );
    
    memset( prime, true, primeLen );                                                                // Assume all numbers are prime
    
    
    // Sieve
    for(uint64_t i = 0, n = 3;  n*n <= limit;  i++, n += 2) {
        if(prime[i]) {														    				    // If a undeleted number is found, it's a prime number

            for(uint64_t m = n*n;  m <= limit;  m += 2*n) {										    // Then delete the multiples of this prime number (because these numbers are composed of prime[n])
				prime[ (m-3) / 2 ] = false;									    		            // Number needs to be uneven, since the list only contains uneven numbers. So m is incremented by 2*n
            }
            
        }
	}
    
    // Count number of found primes
    uint32_t nPrimes = 1;
    for( uint32_t i = 0, n = 3;  i < primeLen;  i++, n +=2 ) {
        if( prime[i] ) {                                                                            // A prime number has been found
            p[nPrimes] = n;                                  // added
            nPrimes++;                                       // added
            if( nPrimes == N ) break;                        // added            
        }
    }


    free(prime);
    return nPrimes;
}
