#include "Lifo.h"
#include <assert.h>
#include <stdlib.h>
#include <values.h>
#include <string.h>
#include <stdio.h>

//type

int StackCtor(Stack *someStackPtr){
	assert(someStackPtr); 

	int statusStackCtor = 0; 
	someStackPtr->stackCapacity = 10; //пусть в начале выделяется на 10, а там посмотрим

	someStackPtr->stackData = (int *)calloc(someStackPtr->stackCapacity, sizeof(int));
																						/
	if (someStackPtr->stackData == nullptr){
		statusStackCtor = 1; 
		return statusStackCtor;
	}

	someStackPtr->stackSize = 0; //удалили данные
	//надо какую-то проверку на size потом делать??

	
	return statusStackCtor; //возвращает код ошибки
}


int StackPush(Stack *someStackPtr, int value){//кладет что-то в конец стека
	assert(someStackPtr); //проверили, существует ли вообще то что нам передали

	int statusStackPush = 0; //если никаких ошибок

	int statusStackMemory = StackResize(someStackPtr);//функция определяет сколько  памяти зарезервировать стеку
	assert(statusStackMemory == 0);

	someStackPtr->stackData[someStackPtr->stackSize] = value;
	someStackPtr->stackSize++;
	
	return statusStackPush;
}


int StackPop(Stack *someStackPtr, int *statusStackPop){//вытаскивает один элемент из стека
	assert(someStackPtr);

	int statusStackMemory = StackResize(someStackPtr);//функция определяет сколько  памяти зарезервировать стеку
	assert(statusStackMemory == 0);

	someStackPtr->stackSize--;
	int valueFromPop = someStackPtr->stackData[someStackPtr->stackSize];

	someStackPtr->stackData[someStackPtr->stackSize] = POISON;

	return valueFromPop;
}


int StackDtor(Stack *someStackPtr){//деструктор стека

	assert(someStackPtr); //проверили, существует ли вообще то что нам передали
	//!TODO StackOK

	int statusStackDtor = 0; //если никаких ошибок

	memset(someStackPtr->stackData, POISON, someStackPtr->stackCapacity);//размер заполняемой области  StackCapacity
																		//или StackSize? почему именно 0xF0
	//так как free не чистит память

	
	free(someStackPtr->stackData);
	someStackPtr->stackData = (int *)FREED_POINTER;// 13 - маленьное число, маркер уже освобожденных укахателей 

	someStackPtr->stackSize = INT_MAX;//самое большое число size_t какое?
	someStackPtr->stackCapacity = 0;//норм????


	return statusStackDtor;
}

//надо бы протестить как-то
int StackResize (Stack *someStackPtr){//управлет размером памяти, выделенной под стек

	int statusStackMemory = 0; //надо возвращать её

	int sizeOfSomeStack     = someStackPtr->stackSize;
	int capacityOfSomeStack = someStackPtr->stackCapacity;
	int *dataOfSomeStack    = someStackPtr->stackData;

	if (capacityOfSomeStack >= LARGE_VOLUME_CRITERION * sizeOfSomeStack){
		int *reallocDataPtr = (int *)realloc(dataOfSomeStack, capacityOfSomeStack * NORMAL_DECREASE_COEFF);
		if (reallocDataPtr == nullptr) {
			return LACK_OF_MEMORY;
		}

		someStackPtr->stackData      = reallocDataPtr;
		someStackPtr->stackCapacity *= NORMAL_DECREASE_COEFF;
	
	}
	 
	else if (capacityOfSomeStack <= SMALL_VOLUME_CRITERION * sizeOfSomeStack){

		if (sizeOfSomeStack > BIG_SIZE_OF_STACK) {

			int *reallocDataPtr = (int *)realloc(dataOfSomeStack, capacityOfSomeStack * SMALL_INCREASE_COEFF);
			if (reallocDataPtr == nullptr) {
				return LACK_OF_MEMORY;
			}

			someStackPtr->stackData      = reallocDataPtr;
			someStackPtr->stackCapacity *= SMALL_INCREASE_COEFF;
		}

		else {
			int *reallocDataPtr = (int *)realloc(dataOfSomeStack, capacityOfSomeStack * NORMAL_INCREASE_COEFF);

			if (reallocDataPtr == nullptr) {
				return LACK_OF_MEMORY;
			}
			someStackPtr->stackData = reallocDataPtr;
			someStackPtr->stackCapacity *= NORMAL_INCREASE_COEFF; 
		}
	}
	
	return statusStackMemory;
}


void StackPrint(Stack someStack){
	PrintSeparator();
	printf("Printing stack...\n\n");

	if (someStack.stackSize == 0) {
		printf("Stack is empty! :)\n\n");
		PrintSeparator();
		return;
	}

	for (int i = 0; i < someStack.stackSize; i++){
		printf("%d\n", someStack.stackData[i]);
	}

	PrintSeparator();

	return;
}

void PrintSeparator(){
	printf("______________________________\n\n");
}
//a почему не раюотает???
