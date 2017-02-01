#include    <inttypes.h>
#include    <stdbool.h>
#include    <stdlib.h>
#include    <string.h>
#include    <sys/time.h>
#include    <stdio.h>


int main( void ) {
    struct timeval  start, end;
    gettimeofday(&start, NULL);
    
    bool* 		prime 	 = (bool*)malloc(sizeof(bool) * (LIMIT-1));								    // says if a number is prime or not. If prime[98] == flase  =>  99 is not prime
    if( prime == NULL ) return 1;

/*------------------------------------------------------------------------------------------------*/

    memset(prime, true, LIMIT-1);

    for(uint64_t i = 2; i*i <= LIMIT; i++) {														// sieve
        if(prime[i-2]) {																			// if a undeleted number is found, it's a prime number

            for(uint64_t j = i*i -2; j <= LIMIT; j += i) {											// then delete the multiples of this prime number (because these numbers are composed of prime[i]
                prime[j] = false;
            }
        }
	}


/*------------------------------------------------------------------------------------------------*/

    gettimeofday(&end, NULL);                                                                       // Nur das Sieb gehört zur Zeitmessung
    
    // Count primes
    uint32_t    nPrimes  = 0;
    for( uint32_t i = 0;  i < LIMIT-1; i++) {
        if( prime[i] ) nPrimes++;
    }   
    
    printf("%.10f,%" PRIu32 "\n", end.tv_sec + end.tv_usec/1e6 - start.tv_sec - start.tv_usec/1e6, nPrimes);
    
	return 0;
}
