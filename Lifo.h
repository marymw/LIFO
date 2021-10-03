#include <string.h>

#define DEBUG 1

#ifdef DEBUG
#define DEBUG_PRINTF(string) printf(string)
#define DEBUG_PRINTF_ONE_ARG(format,var) printf(format,var)
#else
#define DEBUG_PRINTF(string) printf("\n");
#endif

enum Errors {
	NO_ERRORS,      	 //0
	STK_UNDEFINED,       //1
	LACK_OF_MEMORY, 	 //2
	SIZE_LARGER_CAPACITY,//3
	STK_DATA_UNDEFINED,	 //4
	TWICE_CALLED_DTOR,   //5
	CANARY_MISMATCH		 //6
};

enum Constants{
	LARGE_VOLUME_CRITERION,	//2
	SMALL_VOLUME_CRITERION, //1.25
	NORMAL_DECREASE_COEFF,  //0.75
	BIG_SIZE_OF_STACK,		//100
	NORMAL_INCREASE_COEFF,  //1.4
	SMALL_INCREASE_COEFF	//1.1
};

typedef int Type;
typedef unsigned long long int canary_t;

extern const Type POISON        ;
extern const int FREED_POINTER ;
extern const size_t SIZE_T_MAX;
extern const unsigned long long int CANARY_CONST;


struct Stack{
	//void   *stackData;//указатель на начало данных
	//size_t sizeOfTypeOfElements;//это вводит сам пользователь?
	canary_t canary1;
	Type *stackData;
	size_t stackSize;
	size_t stackCapacity; 
	canary_t canary2;
};

#define ASSERT_OK(someStackPtr) if((StackNotOK(someStackPtr, __LINE__, __FILE__, __FUNCTION__))){StackDump(someStackPtr);assert((!"Stack OK"));}

#define StackDump(someStackPtr) {StackDump_(someStackPtr, __LINE__, __FILE__, __FUNCTION__);}

int  StackCtor     (Stack *firstStackPtr);
int  StackDtor     (Stack *firstStackPtr);
int  StackResize   (Stack *someStackPtr);
Type StackTop      (Stack someStack);
int  StackPush     (Stack *firstStackPtr, Type value);
Type StackPop      (Stack *someStackPtr, int *statusStackPop = NULL);
int  StackDump_    (const Stack *someStackPtr, const int line, const char *file, const char *function_name);
int  StackNotOK    (const Stack *someStackPtr, const int line, const char *file, const char *function_name);
void IntStackPrint (Stack someStack);
void PrintElement  (const Stack *someStackPtr);
void PrintSeparator();
void MyMemcpy      (void *newObject, const void *oldObject, size_t numberOfSymbols);

