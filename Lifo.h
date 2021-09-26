include <string.h>

const int POISON        = 0xF0;
const int FREED_POINTER =   13;

struct Stack{
	int   *StackData;//указатель на начало данных
	size_t StackSize;//текущий размер стека
	size_t StackCapacity; //текущая выделенная память под стек
};

int StackCtor  (Stack *FirstStackPtr);
int StackDtor  (Stack *FirstStackPtr);
int StackMemory(Stack *FirstStackPtr);
int StackPush  (Stack *FirstStackPtr, int Value);
int StackPop   (Stack *FirstStackPtr, int *PtrOnValue);
