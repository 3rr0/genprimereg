#include    <primereg.h>
#include    <stdlib.h>
#include    <string.h>
#include	<stdio.h>
#include	<time.h>
#include    <math.h>


void printArr(arr_t arr) {
	for (uint32_t i = 0; i < arr.len; i++) {
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
		Buf[Len] = (rand() % (FUNCTION_END - FUNCTION_START + 1)) + FUNCTION_START;
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
					*(tc_t*)&Buf[Len] = GetRandTc();
					Len += sizeof(tc_t);
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
					*(tc_t*)&Buf[Len] = GetRandTc();
					Len += sizeof(tc_t);
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
					*(tc_t*)&Buf[Len] = GetRandTc();
					Len += sizeof(tc_t);
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
				Buf[Len] = (rand() % (SLB - SLA + 1)) + SLA;
				if (Buf[Len] == SLA)
					AUsed = false;
				else
					BUsed = false;
				Len += 1;
			}
			else {
				Buf[Len] = (rand() % (SLB - SLA + 1)) + SLA;
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
					*(tc_t*)&Buf[Len] = GetRandTc();
					Len += sizeof(tc_t);
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
					*(tc_t*)&Buf[Len] = GetRandTc();
					Len += sizeof(tc_t);
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
					*(tc_t*)&Buf[Len] = GetRandTc();
					Len += sizeof(tc_t);
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
		Buf[Len] = (rand() % (FUNCTION_ONEARG - 1 - FUNCTION_START + 1)) + FUNCTION_START;
		Len += 1;
		Buf[Len] = (rand() % (SLB - SLA + 1)) + SLA;
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
	for (int16_t i = N_FUNCTIONS - 1; i > 0; i--) {
		for (uint16_t j = 0; j < i; j++) {

			//Switch the Elements
			if ((fs[j].fit > fs[j + 1].fit || fs[j].fit < 0.0) && (fs[j +1].fit >= 0.0)) {
				f_t Temp = fs[j];
				fs[j] = fs[j + 1];
				fs[j + 1] = Temp;
			}
		}
	}

	//Seed the randomizer
	srand((unsigned int)time(NULL));

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

bool calcfval(f_t f, uint32_t x, tc_t* val) {
	//Variables
	tc_t SolA = 0.0;
	tc_t SolB = 0.0;
	tc_t TempA = 0.0;
	tc_t TempB = 0.0;

	//Go through the Data
	for (uint16_t i = 0; i < f.term.len; i++) {
		//Find the Function
		switch (f.term.dat[i]) {
		case ADD:
			i++;
			TempA = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
			TempB = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
			if (f.term.dat[i] == SLA)
				SolA = TempA + TempB;
			else
				SolB = TempA + TempB;
			break;
		case SUB:
			i++;
			TempA = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
			TempB = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
			if (f.term.dat[i] == SLA)
				SolA = TempA - TempB;
			else
				SolB = TempA - TempB;
			break;
		case MUL:
			i++;
			TempA = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
			TempB = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
			if (f.term.dat[i] == SLA)
				SolA = TempA * TempB;
			else
				SolB = TempA * TempB;
			break;
		case DIV:
			i++;
			TempA = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
			TempB = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
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
			TempA = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
			if (f.term.dat[i] == SLA)
				SolA = round(TempA);
			else
				SolB = round(TempA);
			break;
		case ABS:
			i++;
			TempA = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
			if (f.term.dat[i] == SLA)
				SolA = fabs(TempA);
			else
				SolB = fabs(TempA);
			break;
		case POW:
			i++;
			TempA = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
			TempB = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
			if ((TempA == 0.0 && TempB == 0) || (TempA < 0.0 && (TempB - (int64_t)TempB) != 0.0) || (TempA == 0.0 && TempB < 0.0))
				return false;
			if (f.term.dat[i] == SLA)
				SolA = pow(TempA, TempB);
			else
				SolB = pow(TempA, TempB);
			break;
		case SIN:
			i++;
			TempA = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
			if (f.term.dat[i] == SLA)
				SolA = sin(TempA);
			else
				SolB = sin(TempA);
			break;
		case COS:
			i++;
			TempA = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
			if (f.term.dat[i] == SLA)
				SolA = cos(TempA);
			else
				SolB = cos(TempA);
			break;
		case TAN:
			i++;
			TempA = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
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
			TempA = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
			if (TempA < -1.0 || TempA > 1.0)
				return false;
			if (f.term.dat[i] == SLA)
				SolA = asin(TempA);
			else
				SolB = asin(TempA);
			break;
		case ACOS:
			i++;
			TempA = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
			if (TempA < -1.0 || TempA > 1.0)
				return false;
			if (f.term.dat[i] == SLA)
				SolA = acos(TempA);
			else
				SolB = acos(TempA);
			break;
		case ATAN:
			i++;
			TempA = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
			if (f.term.dat[i] == SLA)
				SolA = atan(TempA);
			else
				SolB = atan(TempA);
			break;
		case LOG:
			i++;
			TempA = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
			TempB = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
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
			TempA = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
			TempB = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
			if (TempB > -1.0 && TempB < 1.0)
				return false;
			if (f.term.dat[i] == SLA)
				SolA = (tc_t)((int64_t)TempA % (int64_t)TempB);
			else
				SolB = (tc_t)((int64_t)TempA % (int64_t)TempB);
			break;
		case EXP:
			i++;
			TempA = GetParam(f.term.dat, &i, (tc_t)x, SolA, SolB);
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
		default:
			return false;
		}
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
	uint8_t ElementSize = (sizeof(tc_t) * 2) + 3;
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
					i += 1 + sizeof(tc_t);
				else
					i++;
				if (ParA->term.dat[i] == NUM)
					i += 1 + sizeof(tc_t);
				else
					i++;
			}
			else {
				if (ParA->term.dat[i] == NUM)
					i += 1 + sizeof(tc_t);
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
					i += 1 + sizeof(tc_t);
				else
					i++;
				if (ParB->term.dat[i] == NUM)
					i += 1 + sizeof(tc_t);
				else
					i++;
			}
			else {
				if (ParB->term.dat[i] == NUM)
					i += 1 + sizeof(tc_t);
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
	uint16_t St = 0;
	for (uint16_t i = 0; i < ParA->term.len; i++) {
		CurSize = 0;
		St = i;
		if (ParA->term.dat[i] != RET) {
			CurSize++;
			i++;
			if (ParA->term.dat[i - 1] < FUNCTION_ONEARG) {
				if (ParA->term.dat[i] != NUM) {
					CurSize++;
					i++;
				}
				else {
					CurSize += 1 + sizeof(tc_t);
					i += 1 + sizeof(tc_t);
				}
				if (ParA->term.dat[i] != NUM) {
					CurSize++;
					i++;
				}
				else {
					CurSize += 1 + sizeof(tc_t);
					i += 1 + sizeof(tc_t);
				}
			}
			else {
				if (ParA->term.dat[i] != NUM) {
					CurSize++;
					i++;
				}
				else {
					CurSize += 1 + sizeof(tc_t);
					i += 1 + sizeof(tc_t);
				}
			}

			//Copy the Current Element
			memcpy((void*)(Element + (ElementSize * CurElement)), &ParA->term.dat[St], CurSize);
			CurElement++;
		}
		else
			i = ParA->term.len;
	}
	for (uint16_t i = 0; i < ParB->term.len; i++) {
		CurSize = 0;
		St = i;
		if (ParB->term.dat[i] != RET) {
			CurSize++;
			i++;
			if (ParB->term.dat[i - 1] < FUNCTION_ONEARG) {
				if (ParB->term.dat[i] != NUM) {
					CurSize++;
					i++;
				}
				else {
					CurSize += 1 + sizeof(tc_t);
					i += 1 + sizeof(tc_t);
				}
				if (ParB->term.dat[i] != NUM) {
					CurSize++;
					i++;
				}
				else {
					CurSize += 1 + sizeof(tc_t);
					i += 1 + sizeof(tc_t);
				}
			}
			else {
				if (ParB->term.dat[i] != NUM) {
					CurSize++;
					i++;
				}
				else {
					CurSize += 1 + sizeof(tc_t);
					i += 1 + sizeof(tc_t);
				}
			}

			//Copy the Current Element
			memcpy((void*)(Element + (ElementSize * CurElement)), &ParB->term.dat[St], CurSize);
			CurElement++;
		}
		else
			i = ParB->term.len;
	}

	//The ElementArray now includes all the Elements of ParentA & ParentB
	//Mutate the Elements
	for (uint16_t i = 0; i < ElementCount; i++) {
		//Chance of Mutating in Percent
		if (rand() % 101 <= PERC_MUTATION) {
			//Find a new Function
			Element[i * ElementSize] = (rand() % (FUNCTION_END - FUNCTION_START + 1)) + FUNCTION_START;

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
					*(tc_t*)&Element[i * ElementSize + 2] = GetRandTc();
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
					*(tc_t*)&Element[i * ElementSize + 2] = GetRandTc();
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
						*(tc_t*)&Element[i * ElementSize + 2] = GetRandTc();
					}
				}
				else {
					if (T == 0)
						Element[i * ElementSize + 2 + sizeof(tc_t)] = VAR;
					else if (T == 1)
						Element[i * ElementSize + 2 + sizeof(tc_t)] = SLA;
					else if (T == 2)
						Element[i * ElementSize + 2 + sizeof(tc_t)] = SLB;
					else {
						Element[i * ElementSize + 2 + sizeof(tc_t)] = NUM;
						*(tc_t*)&Element[i * ElementSize + 2 + sizeof(tc_t)] = GetRandTc();
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
					if (Element[i * ElementSize + 2 + sizeof(tc_t)] == VAR || Element[i * ElementSize + 2 + sizeof(tc_t)] == NUM)
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
				if (Element[2 + sizeof(tc_t)] != NUM && Element[2 + sizeof(tc_t)] != VAR) {
					if (rand() % 2 == 0)
						Element[2 + sizeof(tc_t)] = VAR;
					else {
						Element[2 + sizeof(tc_t)] = NUM;
						*(tc_t*)&Element[3 + sizeof(tc_t)] = GetRandTc();
					}
				}
			}
			else {
				if (Element[2] != NUM && Element[2] != VAR) {
					if (rand() % 2 == 0)
						Element[2] = VAR;
					else {
						Element[2] = NUM;
						*(tc_t*)&Element[3] = GetRandTc();
					}
				}
			}
		}
		else
			Element[1] = VAR;
	}

	//Create the Child
	Child->term.dat = (uint8_t*)realloc(Child->term.dat, ElementCount * ElementSize);
	Child->term.len = ElementCount * ElementSize;
	memset(Child->term.dat, 0, Child->term.len);
	CurSize = 0;
	memcpy(Child->term.dat, &Element[FirstElement * ElementSize], ElementSize);

	//Get the Current Size
	if (Child->term.dat[0] < FUNCTION_ONEARG) {
		if (Child->term.dat[1] != NUM) {
			if (Child->term.dat[2] == NUM)
				CurSize = 3 + sizeof(tc_t);
			else
				CurSize = 3;
		}
		else {
			if (Child->term.dat[2 + sizeof(tc_t)] == NUM)
				CurSize = 3 + (2 * sizeof(tc_t));
			else
				CurSize = 3 + sizeof(tc_t);
		}
	}
	else {
		if (Child->term.dat[1] != NUM)
			CurSize = 2;
		else
			CurSize = 2 + sizeof(tc_t);
	}
	Child->term.dat[CurSize] = SLA;
	CurSize++;
	AUsed = true;

	//Loop through the Function and add new Elements
	int8_t ParamA = 0;
	int8_t ParamB = 0;
	for (uint16_t i = 0; i < ElementCount; i++) {
		if (CurSize >= Child->term.len - 40)
			i = ElementCount;
		else if (rand() % 101 <= PERC_INHERIT) {
			//Get the Arguments
			if (Element[i * ElementSize] < FUNCTION_ONEARG) {
				ParamA = Element[i * ElementSize + 1];
				if (ParamA == NUM)
					ParamB = Element[i * ElementSize + 2 + sizeof(tc_t)];
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
					*(tc_t*)&Child->term.dat[CurSize] = *(tc_t*)&Element[i * ElementSize + 2];
					CurSize += sizeof(tc_t);
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
							*(tc_t*)&Child->term.dat[CurSize] = *(tc_t*)&Element[i * ElementSize + 3 + sizeof(tc_t)];
						else
							*(tc_t*)&Child->term.dat[CurSize] = *(tc_t*)&Element[i * ElementSize + 3];
						CurSize += sizeof(tc_t);
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
		Child->term.dat[CurSize] = (rand() % (FUNCTION_ONEARG - 1 - FUNCTION_START + 1)) + FUNCTION_START;
		CurSize++;
		Child->term.dat[CurSize] = (rand() % (SLB - SLA + 1)) + SLA;
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
		printFunc(fs[i].term);

		//Start a new line
		printf("\n");
	}
}

tc_t GetParam(uint8_t* d, uint16_t* i, tc_t x, tc_t SolA, tc_t SolB) {
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
	else if (d[*i] == VAR) {
		*i += 1;
		return x;
	}

	//Check if the Following is a number
	*i += 1;
	tc_t Temp = *(tc_t*)&d[*i];
	*i += sizeof(tc_t);
	return Temp;
}

tc_t GetRandTc() {
	//Variables
	tc_t HD = 0.0;
	tc_t LD = 0.0;
	tc_t Sol = 0.0;

	//Create a random High-Value
	HD = (tc_t)(rand() % MAX_RAND_DOUBLE);

	//Create a random Low-Value
	LD = (tc_t)(rand() % (uint64_t)pow(10, MAX_RAND_DOUBLE_DIG));
	for (; LD > 1.0; LD /= 10) {};

	//Create the new Solution
	Sol = HD + LD;

	//Check if the Number is supposed to be negative
	if (rand() % 2 == 0)
		Sol *= -1;

	return Sol;
}

void printFunc(arr_t d) {
	//Go through the Data and print them
	for (uint16_t i = 0; i < d.len; i++) {
		//Print the Function
		switch (d.dat[i]) {
		case ADD:
			i++;
			printf("ADD ");

			//Print the first Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}
			
			//Print the second Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the Store Register
			if (d.dat[i] == SLA)
				printf("SLA ");
			else
				printf("SLB ");
			break;
		case SUB:
			i++;
			printf("SUB ");

			//Print the first Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the second Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the Store Register
			if (d.dat[i] == SLA)
				printf("SLA ");
			else
				printf("SLB ");
			break;
		case MUL:
			i++;
			printf("MUL ");

			//Print the first Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the second Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the Store Register
			if (d.dat[i] == SLA)
				printf("SLA ");
			else
				printf("SLB ");
			break;
		case DIV:
			i++;
			printf("DIV ");

			//Print the first Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the second Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the Store Register
			if (d.dat[i] == SLA)
				printf("SLA ");
			else
				printf("SLB ");
			break;
		case RND:
			i++;
			printf("RND ");

			//Print the first Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the Store Register
			if (d.dat[i] == SLA)
				printf("SLA ");
			else
				printf("SLB ");
			break;
		case ABS:
			i++;
			printf("ABS ");

			//Print the first Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the Store Register
			if (d.dat[i] == SLA)
				printf("SLA ");
			else
				printf("SLB ");
			break;
		case POW:
			i++;
			printf("POW ");

			//Print the first Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the second Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the Store Register
			if (d.dat[i] == SLA)
				printf("SLA ");
			else
				printf("SLB ");
			break;
		case SIN:
			i++;
			printf("SIN ");

			//Print the first Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the Store Register
			if (d.dat[i] == SLA)
				printf("SLA ");
			else
				printf("SLB ");
			break;
		case COS:
			i++;
			printf("COS ");

			//Print the first Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the Store Register
			if (d.dat[i] == SLA)
				printf("SLA ");
			else
				printf("SLB ");
			break;
		case TAN:
			i++;
			printf("TAN ");

			//Print the first Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the Store Register
			if (d.dat[i] == SLA)
				printf("SLA ");
			else
				printf("SLB ");
			break;
		case ASIN:
			i++;
			printf("ASIN ");

			//Print the first Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the Store Register
			if (d.dat[i] == SLA)
				printf("SLA ");
			else
				printf("SLB ");
			break;
		case ACOS:
			i++;
			printf("ACOS ");

			//Print the first Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the Store Register
			if (d.dat[i] == SLA)
				printf("SLA ");
			else
				printf("SLB ");
			break;
		case ATAN:
			i++;
			printf("ATAN ");

			//Print the first Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the Store Register
			if (d.dat[i] == SLA)
				printf("SLA ");
			else
				printf("SLB ");
			break;
		case LOG:
			i++;
			printf("LOG ");

			//Print the first Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the second Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the Store Register
			if (d.dat[i] == SLA)
				printf("SLA ");
			else
				printf("SLB ");
			break;
		case MOD:
			i++;
			printf("MOD ");

			//Print the first Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the second Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the Store Register
			if (d.dat[i] == SLA)
				printf("SLA ");
			else
				printf("SLB ");
			break;
		case EXP:
			i++;
			printf("EXP ");

			//Print the first Parameter
			if (d.dat[i] == SLA) {
				printf("SLA ");
				i++;
			}
			else if (d.dat[i] == SLB) {
				printf("SLB ");
				i++;
			}
			else if (d.dat[i] == VAR) {
				printf("VAR ");
				i++;
			}
			else {
				i++;
				tc_t t = *(tc_t*)&d.dat[i];
				printf("NUM %.5f ", t);
				i += sizeof(tc_t);
			}

			//Print the Store Register
			if (d.dat[i] == SLA)
				printf("SLA ");
			else
				printf("SLB ");
			break;
		case RET:
			i++;
			printf("RET ");

			//Print the Store Register
			if (d.dat[i] == SLA)
				printf("SLA");
			else
				printf("SLB");
			break;
		}
	}
}