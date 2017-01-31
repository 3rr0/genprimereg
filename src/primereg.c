#include    "..\inc\primereg.h"
#include    <primegen.h>
#include    <math.h>
#include	<time.h>
#include    <stdlib.h>
#include	<stdio.h>
#include	<memory.h>

void findp(uint32_t* p) {
	//p is an array of N_PRIMES elements
}
void initf(f_t* f) {
	//Variables
	uint8_t Buf[256];
	uint16_t Len = 0;
	bool AUsed = false;
	bool BUsed = false;
	uint8_t T = 0;

	//Add the first Element
	do {
		//Pick a random new function
		Buf[Len] = (rand() % (FUNCTION_END - FUNCTION_START)) + FUNCTION_START;
		Len += 1;
		
		//Check if SolutionA or SolutionB are currently filled
		if ((AUsed && !BUsed) || (!AUsed && BUsed)) {
			if (Buf[Len - 1] >= FUNCTION_ONEARG) {
				//Get a random number
				T = rand() % 3;

				//Check what Parameter to take
				if (T == 0) {
					Buf[Len] = VAR;
					Len += 1;
				}
				else if (T == 1) {
					Buf[Len] = NUM;
					Len += 1;
					*(double*)&Buf[Len] = (double)(rand() % MAX_RAND_DOUBLE);
					Len += sizeof(double);
				}
				else {
					if (AUsed) {
						Buf[Len] = SLA;
						AUsed = false;
					}
					else {
						Buf[Len] = SLB;
						BUsed = false;
					}
					Len += 1;
				}
			}
			else {
				//Get a random number
				T = rand() % 3;
				
				//Find the first parameter
				if (T == 0) {
					Buf[Len] = VAR;
					Len += 1;
				}
				else if (T == 1) {
					Buf[Len] = NUM;
					Len += 1;
					*(double*)&Buf[Len] = (double)(rand() % MAX_RAND_DOUBLE);
					Len += sizeof(double);
				}
				else {
					if (AUsed) {
						Buf[Len] = SLA;
						AUsed = false;
					}
					else {
						Buf[Len] = SLB;
						BUsed = false;
					}
					Len += 1;
				}

				//Get a random number
				if (AUsed || BUsed)
					T = rand() % 3;
				else
					T = rand() % 2;

				//Find the second parameter
				if (T == 0) {
					Buf[Len] = VAR;
					Len += 1;
				}
				else if (T == 1) {
					Buf[Len] = NUM;
					Len += 1;
					*(double*)&Buf[Len] = (double)(rand() % MAX_RAND_DOUBLE);
					Len += sizeof(double);
				}
				else {
					if (AUsed) {
						Buf[Len] = SLA;
						AUsed = false;
					}
					else {
						Buf[Len] = SLB;
						BUsed = false;
					}
					Len += 1;
				}
			}
		}
		else if (AUsed && BUsed) {
			//Take SolutionA/SolutionB as a parameter
			if (Buf[Len - 1] >= FUNCTION_ONEARG) {
				Buf[Len] = (rand() % SLB - SLA) + SLA;
				if (Buf[Len] == SLA)
					AUsed = false;
				else
					BUsed = false;
				Len += 1;
			}
			else {
				Buf[Len] = (rand() % SLB - SLA) + SLA;
				if (Buf[Len] == SLA)
					Buf[Len + 1] = SLB;
				else
					Buf[Len + 1] = SLA;
				Len += 2;
				AUsed = false;
				BUsed = false;
			}
		}
		else {
			//Check how many arguments are needed
			if (Buf[Len - 1] >= FUNCTION_ONEARG) {
				//Get a random number
				T = rand() % 2;

				//Pick the Arguement
				if (T == 0) {			//Add the independent Variable
					Buf[Len] = VAR;
					Len += 1;
				}
				else {					//Add a random Number
					Buf[Len] = NUM;
					Len += 1;
					*(double*)&Buf[Len] = (double)(rand() % MAX_RAND_DOUBLE);
					Len += sizeof(double);
				}
			}
			else {
				//Get a random number
				T = rand() % 2;

				//Pick the first Arguement
				if (T == 0) {			//Add the independent Variable
					Buf[Len] = VAR;
					Len += 1;
				}
				else {					//Add a random Number
					Buf[Len] = NUM;
					Len += 1;
					*(double*)&Buf[Len] = (double)(rand() % MAX_RAND_DOUBLE);
					Len += sizeof(double);
				}
				//Get a random number
				T = rand() % 2;

				//Pick the second Arguement
				if (T == 0) {			//Add the independent Variable
					Buf[Len] = VAR;
					Len += 1;
				}
				else {					//Add a random Number
					Buf[Len] = NUM;
					Len += 1;
					*(double*)&Buf[Len] = (double)(rand() % MAX_RAND_DOUBLE);
					Len += sizeof(double);
				}
			}
		}

		//Add the Solution to write to
		if (!AUsed) {
			AUsed = true;
			Buf[Len] = SLA;
			Len += 1;
		}
		else {
			BUsed = true;
			Buf[Len] = SLB;
			Len += 1;
		}
	} while (rand() % 101 > PERC_ADD_NEW && Len < 160);

	//Check if both solutions are filled
	if (AUsed && BUsed) {
		//Pick random function to use both parameters
		Buf[Len] = (rand() % (FUNCTION_ONEARG - 1 - FUNCTION_START)) + FUNCTION_START;
		Len += 1;
		Buf[Len] = (rand() % SLB - SLA) + SLA;
		if (Buf[Len] == SLA)
			Buf[Len + 1] = SLB;
		else
			Buf[Len + 1] = SLA;
		UsedA = false;
		UsedB = false;
		Len += 2;
		Buf[Len] = SLA;
		UsedA = true;
	}

	//Add the End
	Buf[Len] = RET;
	Len += 1;
	if (AUsed)
		Buf[Len] = SLA;
	else
		Buf[Len] = SLB;
	Len += 1;

	//Initialize the Element
	f->term.dat = (uint8_t*)malloc(Len);
	memcpy(f->term.dat, Buf, Len);
	f->term.len = Len;
}
void evolve(f_t* fs, uint32_t* p) {
	//Calculate the fitness of the elements
	for (uint16_t i = 0; i < N_FUNCTIONS; i++) {
		calcFitness(&fs[i], p);
	}

	//Find the parents with Bubblesort
	for (uint16_t i = 0; i < N_FUNCTIONS; i++) {
		for (uint16_t j = i; j < N_FUNCTIONS - 1; j++) {

			//Switch the Elements
			if ((fs[j].fit > fs[j + 1].fit) || (fs[i].fit < 0.0)) {
				f_t Temp = fs[j];
				fs[j] = fs[j + 1];
				fs[j + 1] = Temp;
			}
		}
	}

	//Seed the randomizer
	srand(time(NULL));

	//Variables
	uint32_t PA = 0;
	uint32_t PB = 0;

	//Repopulate the Array
	for (uint16_t i = N_PARENTS; i < N_FUNCTIONS; i++) {

		//Find the Parents
		PA = rand() % N_PARENTS;
		PB = rand() % N_PARENTS;

		//Recreate the Elements
		repopulate(&fs[PA], &fs[PB], &fs[i]);
	}
}
void calcFitness(f_t* f, uint32_t* p) {
	//Variables
	double val = 0.0;

	//Init the Fitness
	f->fit = 0.0;

	//Calculate the Fitness
	for (uint32_t i = 0; i < N_PRIMES; i++) {
		if (calcfval(*f, i, &val)) {
			f->fit += pow(val - p[i], 2);
		}
		else {
			f->fit = -1.0;
			break;
		}
	}
}
bool calcfval(f_t f, uint32_t x, double* val) {
	//Variables
	double SolA = 0.0;
	double SolB = 0.0;
	double TempA = 0.0;
	double TempB = 0.0;

	//Go through the Data
	for (uint16_t i = 0; i < f.term.len; i++) {
		//Find the Function
		switch (f.term.dat[i]) {
		case ADD:
			i++;
			TempA = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			TempB = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			if (f.term.dat[i] == SLA)
				SolA = TempA + TempB;
			else
				SolB = TempA + TempB;
			break;
		case SUB:
			i++;
			TempA = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			TempB = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			if (f.term.dat[i] == SLA)
				SolA = TempA - TempB;
			else
				SolB = TempA - TempB;
			break;
		case MUL:
			i++;
			TempA = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			TempB = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			if (f.term.dat[i] == SLA)
				SolA = TempA * TempB;
			else
				SolB = TempA * TempB;
			break;
		case DIV:
			i++;
			TempA = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			TempB = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			if (f.term.dat[i] == SLA) {
				if (TempB == 0.0)
					return false;
				else
					SolA = TempA / TempB;
			}
			else {
				if (TempB == 0.0)
					return false;
				else
					SolB = TempA / TempB;
			}
			break;
		case RND:
			i++;
			TempA = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			if (f.term.dat[i] == SLA)
				SolA = round(TempA);
			else
				SolB = round(TempA);
			break;
		case ABS:
			i++;
			TempA = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			if (f.term.dat[i] == SLA)
				SolA = fabs(TempA);
			else
				SolB = fabs(TempA);
			break;
		case POW:
			i++;
			TempA = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			TempB = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			if ((TempA == 0.0 && TempB == 0) || (TempA < 0.0 && (TempB - (int64_t)TempB) != 0.0) || (TempA == 0.0 && TempB < 0.0))
				return false;
			if (f.term.dat[i] == SLA)
				SolA = pow(TempA, TempB);
			else
				SolB = pow(TempA, TempB);
			break;
		case SIN:
			i++;
			TempA = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			if (f.term.dat[i] == SLA)
				SolA = sin(TempA);
			else
				SolB = sin(TempA);
			break;
		case COS:
			i++;
			TempA = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			if (f.term.dat[i] == SLA)
				SolA = cos(TempA);
			else
				SolB = cos(TempA);
			break;
		case TAN:
			i++;
			TempA = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			if (f.term.dat[i] == SLA) {
				SolA = tan(TempA);
				if (isnan(SolA) || isinf(SolA))
					return false;
			}
			else {
				SolB = tan(TempA);
				if (isnan(SolB) || isinf(SolB))
					return false;
			}
			break;
		case ASIN:
			i++;
			TempA = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			if (a < -1.0 || a > 1.0)
				return false;
			if (f.term.dat[i] == SLA)
				SolA = asin(TempA);
			else
				SolB = asin(TempA);
			break;
		case ACOS:
			i++;
			TempA = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			if (a < -1.0 || a > 1.0)
				return false;
			if (f.term.dat[i] == SLA)
				SolA = acos(TempA);
			else
				SolB = acos(TempA);
			break;
		case ATAN:
			i++;
			TempA = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			if (f.term.dat[i] == SLA)
				SolA = atan(TempA);
			else
				SolB = atan(TempA);
			break;
		case LOG:
			i++;
			TempA = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			TempB = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			if (TempA <= 0.0 || TempB <= 0.0)
				return false;
			TempA = log10(TempA);
			TempB = log10(TempB);
			TempA = TempA / TempB;
			if (f.term.dat[i] == SLA)
				SolA = TempA;
			else
				SolB = TempA;
			break;
		case MOD:
			i++;
			TempA = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			TempB = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			if (TempB == 0.0)
				return false;
			if (f.term.dat[i] == SLA)
				SolA = (int64_t)TempA % (int64_t)TempB;
			else
				SolB = (int64_t)TempA % (int64_t)TempB;
			break;
		case EXP:
			i++;
			TempA = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			if (TempA < 0.0)
				return false;
			if (f.term.dat[i] == SLA)
				SolA = exp(TempA);
			else
				SolB = exp(TempA);
			break;
		case RET:
			i++;
			if (f.term.dat[i] == SLA) {
				if (isnan(SolA))
					return false;
				else if (isinf(SolA))
					return false;
				*val = SolA;
			}
			else {
				if (isnan(SolB))
					return false;
				else if (isinf(SolB))
					return false;
				*val = SolB;
			}
			i = f.term.len;
			break;
		}
	}
	return true;
}
void repopulate(const f_t* ParA, const f_t* ParB, f_t* Child) {
	//implement mutation
}
void PrintData(f_t* fs) {
	//Syntax: Fitness,Length,Data[0] Data[1] ... Data[n]\n
	//Go through the Data and print them
	for (uint32_t i = 0; i < N_FUNCTIONS; i++) {

		//Print the current Fitness
		printf("%.5d,", fs[i].fit);

		//Print the current length
		printf("%" PRIu32 ",", fs[i].term.len);

		//Print the Data
		for (uint32_t j = 0; j < fs[i].term.len; j++) {
			printf("%" PRIu8 " ", fs[i].term.dat[j]);
		}
		printf("\n");
	}
}
double	GetParam(uint8_t* d, uint16_t* i, double x, double SolA, double SolB) {
	//Check if the Following is the solution A
	if (d[*i] == SLA)
	{
		*i += 1;
		return SolA;
	}
	//Check if the Following is the solution B
	else if (d[*i] == SLB)
	{
		*i += 1;
		return SolB;
	}
	//Check if the Following is the Variable X
	else if (Data[*i] == VAR)
	{
		*i += 1;
		return x;
	}
	//Check if the Following is a number
	else
	{
		*i += 1;
		double Temp = *(double*)&d[*i];
		*i += sizeof(double);
		return Temp;
	}
}