#include    "..\inc\primereg.h"
#include	<primereg.h>
#include    <stdlib.h>
#include	<memory.h>

int main( int argc, char* argv[] ) {
	//Variables
	f_t* Data;
	uint32_t* Primes;
	uint16_t Count = 0;

	//Initialize the Data and the Primes
	Data = (f_t*)malloc(sizeof(f_t) * N_FUNCTIONS);
	memset(Data, 0, sizeof(f_t) * N_FUNCTIONS);
	Primes = (uint32_t*)malloc(sizeof(uint32_t) * N_PRIMES);
	memset(Primes, 0, sizeof(uint32_t) * N_PRIMES);

	//Recieve the Primenumbers
	findp(Primes);

	//Seed the randomizer
	srand(time(NULL));

	//Initialize the functions
	for (uint16_t i = 0; i < N_FUNCTIONS; i++) {
		initf(&Data[i]);
	}

	//Main Loop
	do {
		evolve(Data, Primes);
		
		//Print data
		if (++Count >= N_PRINTDATA) {
			PrintData(Data);
			Count = 0;
		}
	} while (1);
    
    return 0;
}