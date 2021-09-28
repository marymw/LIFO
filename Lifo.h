#include <string.h>

#define DEBUG 1

#ifdef DEBUG
#define DEBUG_PRINTF(string) printf(string)
#define DEBUG_PRINTF_ONE_ARG(format,var) printf(format,var)
#else
#define DEBUG_PRINTF(string) printf("\n");
#endif

enum Errors {//а здесь с какой буквы???
	NO_ERRORS,      //0
	STK_UNDEFINED,   //1
	LACK_OF_MEMORY  //2
};

enum Constants{
	LARGE_VOLUME_CRITERION,	//2
	SMALL_VOLUME_CRITERION, //1.25
	NORMAL_DECREASE_COEFF,  //0.75
	BIG_SIZE_OF_STACK,		//100
	NORMAL_INCREASE_COEFF,  //1.4
	SMALL_INCREASE_COEFF	//1.1
};

const int POISON        = 0xF0;
const int FREED_POINTER =   13;

struct Stack{//c маленькой буквы
	int   *stackData;//указатель на начало данных

	size_t stackSize;//текущий размер стека
	size_t stackCapacity; //текущая выделенная память под стек
};

int  StackCtor     (Stack *firstStackPtr);
int  StackDtor     (Stack *firstStackPtr);
int  StackResize   (Stack *someStackPtr);
int  StackPush     (Stack *firstStackPtr, int value);
int  StackPop      (Stack *someStackPtr, int *statusStackPop = NULL);
void StackPrint    (Stack someStack);
void PrintSeparator();
