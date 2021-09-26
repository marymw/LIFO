#include "Lifo.h"
#include <assert.h>
#include <stdlib.h>
#include <values.h>
#include <string.h>
#include <stdio.h>

//STACKRESIZE
//type

int StackCtor(Stack *SomeStackPtr){//конструктор стека, создает стек, возвращает ошибку
	assert(SomeStackPtr); //проверили, существует ли вообще то что нам передали

	int StatusStackCtor = 0; //если никаких ошибок
	SomeStackPtr->StackCapacity = 10; //пусть в начале выделяется на 10, а там посмотрим

	SomeStackPtr->StackData = (int *)calloc(SomeStackPtr->StackCapacity, sizeof(int));//какой размер передавать стеку 
																						//в консрукторе

	if (SomeStackPtr->StackData == nullptr){
		StatusStackCtor = 1; //ошибка связанная с выделением памяти
		return StatusStackCtor;
	}

	SomeStackPtr->StackSize = 0; //удалили данные
	//надо какую-то проверку на size потом делать??

	
	return StatusStackCtor; //возвращает код ошибки
}


int StackPush(Stack *SomeStackPtr, int Value){//кладет что-то в конец стека
	assert(SomeStackPtr); //проверили, существует ли вообще то что нам передали

	int StatusStackPush = 0; //если никаких ошибок

	int StatusStackMemory = StackResize(SomeStackPtr);//функция определяет сколько  памяти зарезервировать стеку
	assert(StatusStackMemory == 0);

	SomeStackPtr->StackData[SomeStackPtr->StackSize] = Value;
	SomeStackPtr->StackSize++;
	
	return StatusStackPush;
}


int StackPop(Stack *SomeStackPtr, int *StatusStackPop){//вытаскивает один элемент из стека
	assert(SomeStackPtr);

	DEBUG_PRINTF_ONE_ARG("I am in beggining of function %s\n", __FUNCTION__);
/* из-за этого куска всё падает
	if (SomeStackPtr == NULL){ //проверили, существует ли вообще то что нам передали
		DEBUG_PRINTF("в операторе if \n");
		*StatusStackPop = STK_UNDEFINED;
		//а как выйти в этом случае??
	}
	else{
		*StatusStackPop = NO_ERRORS;
		DEBUG_PRINTF("в операторе if \n");
	}
	//int StatusStackPop = 0; //если никаких ошибок
*/


	DEBUG_PRINTF_ONE_ARG("I am in function %s\n", __FUNCTION__);

	int StatusStackMemory = StackResize(SomeStackPtr);//функция определяет сколько  памяти зарезервировать стеку
	assert(StatusStackMemory == 0);

	SomeStackPtr->StackSize--;
	int ValueFromPop = SomeStackPtr->StackData[SomeStackPtr->StackSize];

	SomeStackPtr->StackData[SomeStackPtr->StackSize] = POISON;

	DEBUG_PRINTF_ONE_ARG("I am in the end function %s\n", __FUNCTION__);

	return ValueFromPop;
}


int StackDtor(Stack *SomeStackPtr){//деструктор стека

	assert(SomeStackPtr); //проверили, существует ли вообще то что нам передали

	int StatusStackDtor = 0; //если никаких ошибок

	//залили ядом
	memset(SomeStackPtr->StackData, POISON, SomeStackPtr->StackCapacity);//размер заполняемой области  StackCapacity
																		//или StackSize? почему именно 0xF0
	//так как free не чистит память

	
	free(SomeStackPtr->StackData);
	SomeStackPtr->StackData = (int *)FREED_POINTER;// 13 - маленьное число, маркер уже освобожденных укахателей 

	SomeStackPtr->StackSize = INT_MAX;//самое большое число size_t какое?
	SomeStackPtr->StackCapacity = 0;//норм????


	return StatusStackDtor;
}


int StackResize (Stack *SomeStackPtr){//управлет размером памяти, выделенной под стек
	int StatusStackMemory = 0; //если всё ок
	//printf("Функция %s ещё не реализована\n хрум - хрум\n", __FUNCTION__);

	if (SomeStackPtr->StackCapacity >= LARGE_VOLUME_CRITERION * SomeStackPtr->StackSize){
		int *ReallocDataPtr = (int *)realloc(SomeStackPtr->StackData, 
												  SomeStackPtr->StackCapacity * SMALL_COEFFICIENT);
		if (ReallocDataPtr == nullptr) {
			return LACK_OF_MEMORY;
		}

		SomeStackPtr->StackData = ReallocDataPtr;
		SomeStackPtr->StackCapacity *= SMALL_COEFFICIENT;
	
	}
	else if (SomeStackPtr->StackCapacity <= SMALL_VOLUME_CRITERION * SomeStackPtr->StackSize){
		if (SomeStackPtr->StackSize > VERY_BIG_SIZE_OF_STACK) {

			int *ReallocDataPtr = (int *)realloc(SomeStackPtr->StackData, 
													  SomeStackPtr->StackCapacity * (VERY_SMALL_COEFFICIENT + 1));
			if (ReallocDataPtr == nullptr) {
				return LACK_OF_MEMORY;
			}

			SomeStackPtr->StackData = ReallocDataPtr;
			SomeStackPtr->StackCapacity *= VERY_SMALL_COEFFICIENT + 1;//X + X*A = X(A+1)
		}

		else if (SomeStackPtr->StackSize > BIG_SIZE_OF_STACK) {

			int *ReallocDataPtr = (int *)realloc(SomeStackPtr->StackData, 
													  SomeStackPtr->StackCapacity * (SMALL_COEFFICIENT + 1));
			if (ReallocDataPtr == nullptr) {
				return LACK_OF_MEMORY;
			}

			SomeStackPtr->StackData = ReallocDataPtr;
			SomeStackPtr->StackCapacity *= SMALL_COEFFICIENT + 1;//X + X*A = X(A+1)
		}

		else {
			int *ReallocDataPtr = (int *)realloc(SomeStackPtr->StackData, SomeStackPtr->StackCapacity);

			if (ReallocDataPtr == nullptr) {
				return LACK_OF_MEMORY;
			}
			SomeStackPtr->StackData = ReallocDataPtr;
			SomeStackPtr->StackCapacity *= 2;//проверка если налл 
		}
	}
	
	return StatusStackMemory;
}


void StackPrint(Stack SomeStack){
	PrintSeparator();
	printf("Printing stack...\n\n");

	if (SomeStack.StackSize == 0) {
		printf("Stack is empty! :)\n\n");
		PrintSeparator();
		return;
	}

	for (int i = 0; i < SomeStack.StackSize; i++){
		printf("%d\n", SomeStack.StackData[i]);
	}

	PrintSeparator();

	return;
}

void PrintSeparator(){
	printf("______________________________\n\n");
}
//a почему не раюотает???
