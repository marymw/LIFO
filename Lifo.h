#include <string.h>

#define DEBUG 1

#ifdef DEBUG
#define DEBUG_PRINTF(string) printf(string)
#define DEBUG_PRINTF_ONE_ARG(format,var) printf(format,var)
#else
#define DEBUG_PRINTF(string) printf("\n");
#endif

enum Errors {
	NO_ERRORS,      //0
	STK_UNDEFINED   //1
};

const int POISON        = 0xF0;
const int FREED_POINTER =   13;

struct Stack{
	int   *StackData;//указатель на начало данных
	size_t StackSize;//текущий размер стека
	size_t StackCapacity; //текущая выделенная память под стек
};

int  StackCtor     (Stack *FirstStackPtr);
int  StackDtor     (Stack *FirstStackPtr);
int  StackMemory   (Stack *FirstStackPtr);
int  StackPush     (Stack *FirstStackPtr, int Value);
int  StackPop      (Stack *SomeStackPtr, int *StatusStackPop = NULL);
void StackPrint    (Stack SomeStack);
void PrintSeparator();
