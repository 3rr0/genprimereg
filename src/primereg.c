#include    <primereg.h>
#include    <stdlib.h>
#include    <string.h>
#include	<stdio.h>
#include	<time.h>
#include    <math.h>



void printArr( arr_t arr ) {
    for( uint32_t i = 0;  i < arr.len;  i++ ) {
        printf("%" PRIu8 ", ", arr.dat[i]);
    }
    
    printf("\n");
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
					*(double*)&Buf[Len] = (double)((rand() % MAX_RAND_DOUBLE) + ((rand() % MAX_RAND_DOUBLE) / (pow(10, MAX_RAND_DOUBLE_DIG))));
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
					*(double*)&Buf[Len] = (double)((rand() % MAX_RAND_DOUBLE) + ((rand() % MAX_RAND_DOUBLE) / (pow(10, MAX_RAND_DOUBLE_DIG))));
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
					*(double*)&Buf[Len] = (double)((rand() % MAX_RAND_DOUBLE) + ((rand() % MAX_RAND_DOUBLE) / (pow(10, MAX_RAND_DOUBLE_DIG))));
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
					*(double*)&Buf[Len] = (double)((rand() % MAX_RAND_DOUBLE) + ((rand() % MAX_RAND_DOUBLE) / (pow(10, MAX_RAND_DOUBLE_DIG))));
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
					*(double*)&Buf[Len] = (double)((rand() % MAX_RAND_DOUBLE) + ((rand() % MAX_RAND_DOUBLE) / (pow(10, MAX_RAND_DOUBLE_DIG))));
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
					*(double*)&Buf[Len] = (double)((rand() % MAX_RAND_DOUBLE) + ((rand() % MAX_RAND_DOUBLE) / (pow(10, MAX_RAND_DOUBLE_DIG))));
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
	} while (rand() % 101 <= PERC_ADD_NEW && Len < 160);

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
		AUsed = false;
		BUsed = false;
		Len += 2;
		Buf[Len] = SLA;
		AUsed = true;
		Len++;
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
			if (TempA < -1.0 || TempA > 1.0)
				return false;
			if (f.term.dat[i] == SLA)
				SolA = asin(TempA);
			else
				SolB = asin(TempA);
			break;
		case ACOS:
			i++;
			TempA = GetParam(f.term.dat, &i, (double)x, SolA, SolB);
			if (TempA < -1.0 || TempA > 1.0)
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
		default
			return false;
	}
	return true;
}

void repopulate(const f_t* ParA, const f_t* ParB, f_t* Child) {
	/*
	Function:
	Find Elements from ParentA & ParentB
		->	One Element is for example: add var num 64.345
	Mutate them
		-> Add them together to a new function
	*/

	//Variables
	uint16_t ElementCount = 0;
	uint16_t CurElement = 0;
	uint8_t ElementSize = (sizeof(double) * 2) + 3;
	uint8_t* Element = 0;
	int16_t FirstElement = -1;
	bool AUsed = false;
	bool BUsed = false;
	
	//Calculate the ammount of Elements needed
	for (uint16_t i = 0; i < ParA->term.len; i++) {
		//Check if the Function has ended
		if (ParA->term.dat[i] != RET) {
			ElementCount++;
			i++;
			if (ParA->term.dat[i - 1] < FUNCTION_ONEARG) {
				if (ParA->term.dat[i] == NUM)
					i += 1 + sizeof(double);
				else
					i++;
				if (ParA->term.dat[i] == NUM)
					i += 1 + sizeof(double);
				else
					i++;
			}
			else {
				if (ParA->term.dat[i] == NUM)
					i += 1 + sizeof(double);
				else
					i++;
			}
		}
		else
			i = ParA->term.len;
	}
	for (uint16_t i = 0; i < ParB->term.len; i++) {
		//Check if the Function has ended
		if (ParB->term.dat[i] != RET) {
			ElementCount++;
			i++;
			if (ParB->term.dat[i - 1] < FUNCTION_ONEARG) {
				if (ParB->term.dat[i] == NUM)
					i += 1 + sizeof(double);
				else
					i++;
				if (ParB->term.dat[i] == NUM)
					i += 1 + sizeof(double);
				else
					i++;
			}
			else {
				if (ParB->term.dat[i] == NUM)
					i += 1 + sizeof(double);
				else
					i++;
			}
		}
		else
			i = ParB->term.len;
	}

	//Allocate the Array and copy the Elements into it
	Element = (uint8_t*)malloc(ElementCount * ElementSize);
	memset(Element, 0, ElementCount * ElementSize);

	//Read in the Elements of the ParentA
	uint8_t CurSize = 0;
	for (uint16_t i = 0; i < ParA->term.len; i++) {
		CurSize = 0;
		if (ParA->term.dat[i] != RET) {
			CurSize++;
			if (ParA->term.dat[i] < FUNCTION_ONEARG) {
				if (ParA->term.dat[i + CurSize] != NUM)
					CurSize++;
				else
					CurSize += 1 + sizeof(double);
				if (ParA->term.dat[i + CurSize] != NUM)
					CurSize++;
				else
					CurSize += 1 + sizeof(double);
			}
			else {
				if (ParA->term.dat[i + CurSize] != NUM)
					CurSize++;
				else
					CurSize += 1 + sizeof(double);
			}

			//Copy the Current Element
			memcpy((void*)(Element + (ElementSize * CurElement)), &ParA->term.dat[i], CurSize);
			CurElement++;
		}
	}
	for (uint16_t i = 0; i < ParB->term.len; i++) {
		CurSize = 0;
		if (ParB->term.dat[i] != RET) {
			CurSize++;
			if (ParB->term.dat[i] < FUNCTION_ONEARG) {
				if (ParB->term.dat[i + CurSize] != NUM)
					CurSize++;
				else
					CurSize += 1 + sizeof(double);
				if (ParB->term.dat[i + CurSize] != NUM)
					CurSize++;
				else
					CurSize += 1 + sizeof(double);
			}
			else {
				if (ParB->term.dat[i + CurSize] != NUM)
					CurSize++;
				else
					CurSize += 1 + sizeof(double);
			}

			//Copy the Current Element
			memcpy((void*)(Element + (ElementSize * CurElement)), &ParB->term.dat[i], CurSize);
			CurElement++;
		}
	}

	//The ElementArray now includes all the Elements of ParentA & ParentB
	//Mutate the Elements
	for (uint16_t i = 0; i < ElementCount; i++) {
		//Chance of Mutating in Percent
		if (rand() % 101 <= PERC_MUTATION) {
			//Find a new Function
			Element[i * ElementSize] = (rand() % (FUNCTION_END - FUNCTION_START)) + FUNCTION_START;
			
			//Find the new Arguments
			if (Element[i * ElementSize] >= FUNCTION_ONEARG) {
				uint8_t T = rand() % 4;
				if (T == 0)
					Element[i * ElementSize + 1] = VAR;
				else if (T == 1)
					Element[i * ElementSize + 1] = SLA;
				else if (T == 2)
					Element[i * ElementSize + 1] = SLB;
				else {
					Element[i * ElementSize + 1] = NUM;
					*(double*)&Element[i * ElementSize + 2] = (double)((rand() % MAX_RAND_DOUBLE) + ((rand() % MAX_RAND_DOUBLE) / (pow(10, MAX_RAND_DOUBLE_DIG))));
				}
			}
			else {
				uint8_t T = rand() % 4;
				if (T == 0)
					Element[i * ElementSize + 1] = VAR;
				else if (T == 1)
					Element[i * ElementSize + 1] = SLA;
				else if (T == 2)
					Element[i * ElementSize + 1] = SLB;
				else {
					Element[i * ElementSize + 1] = NUM;
					*(double*)&Element[i * ElementSize + 2] = (double)((rand() % MAX_RAND_DOUBLE) + ((rand() % MAX_RAND_DOUBLE) / (pow(10, MAX_RAND_DOUBLE_DIG))));
				}
				T = rand() % 4;
				if (Element[i * ElementSize + 1] != NUM) {
					if (T == 0)
						Element[i * ElementSize + 2] = VAR;
					else if (T == 1 && Element[i * ElementSize + 1] != SLA)
						Element[i * ElementSize + 2] = SLA;
					else if (T == 2 && Element[i * ElementSize + 1] != SLB)
						Element[i * ElementSize + 2] = SLB;
					else {
						Element[i * ElementSize + 2] = NUM;
						*(double*)&Element[i * ElementSize + 2] = (double)((rand() % MAX_RAND_DOUBLE) + ((rand() % MAX_RAND_DOUBLE) / (pow(10, MAX_RAND_DOUBLE_DIG))));
					}
				}
				else {
					if (T == 0)
						Element[i * ElementSize + 2 + sizeof(double)] = VAR;
					else if (T == 1)
						Element[i * ElementSize + 2 + sizeof(double)] = SLA;
					else if (T == 2)
						Element[i * ElementSize + 2 + sizeof(double)] = SLB;
					else {
						Element[i * ElementSize + 2 + sizeof(double)] = NUM;
						*(double*)&Element[i * ElementSize + 2 + sizeof(double)] = (double)((rand() % MAX_RAND_DOUBLE) + ((rand() % MAX_RAND_DOUBLE) / (pow(10, MAX_RAND_DOUBLE_DIG))));
					}
				}
			}
		}

		//Check if the Element could be the first one
		if (Element[i * ElementSize + 1] == VAR || Element[i * ElementSize + 1] == NUM) {
			if (Element[i * ElementSize] >= FUNCTION_ONEARG)
				FirstElement = i;
			else {
				if (Element[i * ElementSize + 1] == NUM) {
					if (Element[i * ElementSize + 2 + sizeof(double)] == VAR || Element[i * ElementSize + 2 + sizeof(double)] == NUM)
						FirstElement = i;
				}
				else {
					if (Element[i * ElementSize + 2] == VAR || Element[i * ElementSize + 2] == NUM)
						FirstElement = i;
				}
			}
		}
	}

	//Build the new Child
	//Check if there is a new Element
	if (FirstElement == -1) {
		FirstElement = 0;
		if (Element[0] < FUNCTION_ONEARG) {
			if (Element[1] != NUM && Element[1] != VAR) {
				Element[1] = VAR;
			}
			if (Element[1] == NUM) {
				if (Element[2 + sizeof(double)] != NUM && Element[2 + sizeof(double)] != VAR) {
					if (rand() % 2 == 0)
						Element[2 + sizeof(double)] = VAR;
					else {
						Element[2 + sizeof(double)] = NUM;
						*(double*)&Element[3 + sizeof(double)] = (double)((rand() % MAX_RAND_DOUBLE) + ((rand() % MAX_RAND_DOUBLE) / (pow(10, MAX_RAND_DOUBLE_DIG))));
					}
				}
			}
			else {
				if (Element[2] != NUM && Element[2] != VAR) {
					if (rand() % 2 == 0)
						Element[2] = VAR;
					else {
						Element[2] = NUM;
						*(double*)&Element[3] = (double)((rand() % MAX_RAND_DOUBLE) + ((rand() % MAX_RAND_DOUBLE) / (pow(10, MAX_RAND_DOUBLE_DIG))));
					}
				}
			}
		}
		else
			Element[1] = VAR;
	}

	//Create the Child
	Child->term.dat = (uint8_t)realloc(Child->term.dat, ElementCount * ElementSize);
	Child->term.len = ElementCount * ElementSize;
	memset(Child->term.dat, 0, Child->term.len);
	CurSize = 0;
	memcpy(Child->term.dat, Element[FirstElement * ElementSize], ElementSize);

	//Get the Current Size
	if (Child->term.dat[0] < FUNCTION_ONEARG) {
		if (Child->term.dat[1] != NUM) {
			if (Child->term.dat[2] == NUM)
				CurSize = 3 + sizeof(double);
			else
				CurSize = 3;
		}
		else {
			if (Child->term.dat[2 + sizeof(double)] == NUM)
				CurSize = 3 + (2 * sizeof(double));
			else
				CurSize = 3 + sizeof(double);
		}
	}
	else {
		if (Child->term.dat[1] != NUM)
			CurSize = 2;
		else
			CurSize = 2 + sizeof(double);
	}
	Child->term.dat[CurSize] = SLA;
	CurSize++;
	AUsed = true;

	//Loop through the Function and add new Elements
	int8_t ParamA = 0;
	int8_t ParamB = 0;
	for (uint16_t i = 0; i < ElementCount; i++) {
		if ((rand() % 101) > PERC_INHERIT || CurSize >= Child->term.len - 40)
			i = ElementCount;
		else {
			//Get the Arguments
			if (Element[i * ElementSize] < FUNCTION_ONEARG) {
				ParamA = Element[i * ElementSize + 1];
				if (ParamA == NUM)
					ParamB = Element[i * ElementSize + 2 + sizeof(double)];
				else
					ParamB = Element[i * ElementSize + 2];
			}
			else {
				ParamA = Element[i * ElementSize + 1];
				ParamB = -1;
			}

			//Check if the Arguments are available
			if (!((AUsed && BUsed && ParamA != SLA && ParamB != SLA) || (ParamA == SLA && !AUsed) || (ParamA == SLB && !BUsed) || (ParamB == SLA && !AUsed) || (ParamB == SLB && !BUsed) || (ParamA == ParamB && (ParamA == SLA || ParamA == SLB)))) {
				Child->term.dat[CurSize] = Element[i * ElementSize];
				CurSize++;
				Child->term.dat[CurSize] = ParamA;
				CurSize++;
				if (ParamA == SLA)
					AUsed = false;
				else if (ParamA == SLB)
					BUsed = false;
				else if (ParamA == NUM) {
					*(double*)&Child->term.dat[CurSize] = *(double*)&Element[i * ElementSize + 2];
					CurSize += sizeof(double);
				}
				if (ParamB != -1) {
					Child->term.dat[CurSize] = ParamB;
					CurSize++;
					if (ParamB == SLA)
						AUsed = false;
					else if (ParamB == SLB)
						BUsed = false;
					else if (ParamB == NUM) {
						if (ParamA == NUM)
							*(double*)&Child->term.dat[CurSize] = *(double*)&Element[i * ElementSize + 3 + sizeof(double)];
						else
							*(double*)&Child->term.dat[CurSize] = *(double*)&Element[i * ElementSize + 3];
						CurSize += sizeof(double);
					}
				}

				//Add the End to the Function
				if (!AUsed) {
					Child->term.dat[CurSize] = SLA;
					AUsed = true;
				}
				else {
					Child->term.dat[CurSize] = SLB;
					BUsed = true;
				}
				CurSize++;
			}
		}
	}

	//Check if both solutions are filled
	if (AUsed && BUsed) {
		//Pick random function to use both parameters
		Child->term.dat[CurSize] = (rand() % (FUNCTION_ONEARG - 1 - FUNCTION_START)) + FUNCTION_START;
		CurSize++;
		Child->term.dat[CurSize] = (rand() % SLB - SLA) + SLA;
		if (Child->term.dat[CurSize] == SLA)
			Child->term.dat[CurSize + 1] = SLB;
		else
			Child->term.dat[CurSize + 1] = SLA;
		AUsed = false;
		BUsed = false;
		CurSize += 2;
		Child->term.dat[CurSize] = SLA;
		AUsed = true;
		CurSize++;
	}

	//Add the End
	Child->term.dat[CurSize] = RET;
	CurSize++;
	if (AUsed)
		Child->term.dat[CurSize] = SLA;
	else
		Child->term.dat[CurSize] = SLB;
	CurSize++;

	//Adjust the Memory
	Child->term.dat = (uint8_t*)realloc(Child->term.dat, CurSize);
	Child->term.len = CurSize;

	//Free the Memory
	free(Element);
}

void PrintData(f_t* fs) {
	//Syntax: Fitness,Length,Data[0] Data[1] ... Data[n]\n
	//Go through the Data and print them
	for (uint32_t i = 0; i < N_FUNCTIONS; i++) {

		//Print the current Fitness
		printf("%.5f,", fs[i].fit);

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
	if (d[*i] == SLA) {
		*i += 1;
		return SolA;
	}
	//Check if the Following is the solution B
	else if (d[*i] == SLB) {
		*i += 1;
		return SolB;
	}
	//Check if the Following is the Variable X
	else if (d[*i] == VAR){
		*i += 1;
		return x;
	}
	
    //Check if the Following is a number
    *i += 1;
    double Temp = *(double*)&d[*i];
    *i += sizeof(double);
    return Temp;
}