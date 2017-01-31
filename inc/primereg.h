#pragma     once
#include    <inttypes.h>
#define N_FUNCTIONS			32		//Number of Functions per generation
#define N_PARENTS			6		//Number of parents to pick
#define N_PRIMES			16		//Number of primes
#define N_PRINTDATA			256		//Number of loops til print of data
#define PERC_MUTATION		2		//Chance in percent of a mutation of a function
#define FUNCTION_START		0x10	//Number of first function
#define FUNCTION_ONEARG		0x17	//Number of first function to take one argument
#define FUNCTION_END		0x1f	//Number of last function
#define MAX_RAND_DOUBLE		65536	//Maximum double to be picked as random number
#define PERC_ADD_NEW		60		//Percentage of adding a new function on creation
#define DEBUG
#ifdef DEBUG
#undef N_PRINTDATA
#define N_PRINTDATA 1
#endif

/*
Syntax:	FUNCTIONS - ParamA - (evtl. ParamB) - SaveVal

Example:
y=(x + 4)^6

Syntax:			ADD VAR NUM 4.0                     SLA POW SLA NUM 6.0                     SLA RET SLA
In Bytes(hex):	10	00	03	00 00 00 00 00 00 00 04 01  16  01  03  00 00 00 00 00 00 00 06 01  ff  01

Explained:
-> add(x + (Number)4) and save in SLA
-> power(Value in Sla to power of (Number)6)) save in SLA
-> End with y in SLA
*/

//types
typedef struct arr {
	uint8_t* dat;
	uint32_t len;
} arr_t;
typedef struct function {
	arr_t  term;
	double fit;
} f_t;
enum FUNCTIONS {
	VAR,			//Usage of Variable x
	SLA,			//Temp. Solution A
	SLB,			//Temp. Solution B
	NUM,			//following is a double
	ADD = 0x10,		//double add(double a, double b)			-> a + b
	SUB,			//double substract(double a, double b)		-> a - b
	MUL,			//double multiply(double a, double b)		-> a * b
	DIV,			//double divide(double a, double b)			-> a / b						- if b == 0 return -1.0
	POW,			//double power(double a, double b)			-> a^b							- if (a == 0 && b == 0) || (a < 0 && b != integer) || (a == 0.0 && b < 0.0) return -1.0
	LOG,			//double logarithm(double a, double b)		-> log(a) / log(b)				- a <= 0 or b <= 0 return -1.0
	MOD,			//double modulo(double a, double b)			-> (int64_t)a % (int64_t)b		- if b == 0 return -1.0
	RND,			//double round(double a)					-> (int64_t)(a + 0.5)
	ABS,			//double absolute(double a)					-> +a
	SIN,			//double sin(double a)						-> sin(a)
	COS,			//double cos(double a)						-> cos(a)
	TAN,			//double tan(double a)						-> tan(a)						- if nan or inf return -1.0
	ASIN,			//double arcsin(double a)					-> asin(a)						- if a < -1.0 or a > 1.0 return -1.0
	ACOS,			//double arccos(double a)					-> acos(a)						- if a < -1.0 or a > 1.0 return -1.0
	ATAN,			//double arctan(double a)					-> atan(a)
//FAC,				//double factorial(double a)				-> (int64_t)a!					- evtl. limitations in size
	EXP,			//double exponent(double a)					-> e^(int64_t)a					- if a < 0 return -1.0
	RET = 0xff		//end of function
};

//functions
void    findp       (uint32_t* p);
void	initf		(f_t* f);
void    evolve      (f_t* fs, uint32_t* p);
void	calcFitness	(f_t* f, uint32_t* p);
bool	calcfval	(f_t f, uint32_t x, double* val);
void	repopulate	(const f_t* ParA, const f_t* ParB, f_t* Child);
void	PrintData	(f_t* fs);
double	GetParam	(uint8_t* d, uint16_t* i, double x, double SolA, double SolB);