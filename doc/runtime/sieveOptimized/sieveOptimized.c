/*
    Written 13.2.16 by SimPB

    Math.h needs to be linked with the flag -lm while compiling.
	Uses  2*LIMIT / (LIMIT - 2)  times less RAM than the version with even numbers:

	*
	*	Ges.:  k: Proportionalitätsfaktor
	*
	*		RAM_withEven = k * RAM_withoutEven
	*	<=> LIMIT 		 = k * ( LIMIT/2 - 1)
	*					 = k*LIMIT / 2  - k
	*					 = k*LIMIT / 2  - 2k/2														// Auf Hauptnenner bringen
	*					 = (LIMIT-2)k / 2
	*	<=>	2*LIMIT		 = (LIMIT-2)k
	*	<=>	k			 = 2*LIMIT / (LIMIT-2)
	*


	- bereits gestrichene vielfache nicht mehr streichen. Wie?
		- würde mehr Aufwand als Zeitgewinn bringen
*/

#include    <inttypes.h>
#include    <stdbool.h>
#include    <stdlib.h>
#include    <string.h>
#include    <sys/time.h>
#include    <stdio.h>
#include    <math.h>


int main( void ) {
    struct timeval  start, end;
    gettimeofday(&start, NULL);
    																		// number of primes from 0 to LIMIT
    uint32_t	primeLen = ceil(LIMIT / 2) - 1;														// ceil(LIMIT/2) == #unevenNumbers;  #unevenNumbers-1 because 1 is not prime;  The List now starts with 3
    bool* 		prime 	 = (bool*)malloc(sizeof(bool) * primeLen);								    // says if a number is prime or not. If prime[48] == flase  =>  99 is not prime
    if( prime == NULL ) return 1;

    memset(prime, true, primeLen);


/*------------------------------------------------------------------------------------------------*/


    for(uint64_t i = 0, n = 3;  n*n <= LIMIT;  i++, n += 2) {										// sieve
        if(prime[i]) {																				// if a undeleted number is found, it's a prime number


            for(uint64_t m = n*n;  m <= LIMIT;  m += 2*n) {										    // then delete the multiples of this prime number (because these numbers are composed of prime[n])
				prime[ (m-3) / 2 ] = false;														    // number needs to be uneven, since the list only contains uneven numbers. So m is incremented by 2*n
            }
        }
	}


/*------------------------------------------------------------------------------------------------*/

    gettimeofday(&end, NULL);                                                                       // Nur das Sieb gehört zur Zeitmessung
    
    // Count primes
    uint32_t    nPrimes  = 1;
    for( uint32_t i = 0;  i < primeLen; i++) {
        if( prime[i] ) nPrimes++;
    }   
    
    printf("%.10f,%" PRIu32 "\n", end.tv_sec + end.tv_usec/1e6 - start.tv_sec - start.tv_usec/1e6, nPrimes);
    
	return 0;
}
