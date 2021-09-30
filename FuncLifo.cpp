#include "Lifo.h"
#include <assert.h>
#include <stdlib.h>
#include <values.h>
#include <string.h>
#include <stdio.h>

//type
//норм
int StackCtor(Stack *someStackPtr){

	if (someStackPtr == 0) {
		return StackNotOK(someStackPtr);
	} 

	someStackPtr->stackCapacity = 10; //пусть в начале выделяется на 10, а там посмотрим
	someStackPtr->stackData     = (int *)calloc(someStackPtr->stackCapacity, sizeof(int));
	memset(someStackPtr->stackData, POISON, someStackPtr->stackCapacity);
																						
	if (someStackPtr->stackData == nullptr){ 
		return StackNotOK(someStackPtr);
	}

	someStackPtr->stackSize = 0; //удалили данные
	
	ASSERT_OK(someStackPtr);

	return NO_ERRORS; 
}

//норм
int StackPush(Stack *someStackPtr, const int value){ 

	ASSERT_OK(someStackPtr);

	int statusStackMemory = StackResize(someStackPtr);
	assert(statusStackMemory == 0);//надо чем-то заменить?

	someStackPtr->stackData[someStackPtr->stackSize] = value;
	someStackPtr->stackSize++;

	ASSERT_OK(someStackPtr);
	
	return NO_ERRORS;
}

//норм
int StackPop(Stack *someStackPtr, int *statusStackPop){

	ASSERT_OK(someStackPtr);

	int statusStackMemory = StackResize(someStackPtr);
	assert(statusStackMemory == 0);

	someStackPtr->stackSize--;
	int sizeOfSomeStack = someStackPtr->stackSize;//int sizeOfSomeStack = (--someStackPtr->stackSize);

	int valueFromPop = someStackPtr->stackData[sizeOfSomeStack];

	someStackPtr->stackData[sizeOfSomeStack] = POISON;

	ASSERT_OK(someStackPtr);

	return valueFromPop;
}

//НОРМ
int StackDtor(Stack *someStackPtr){//деструктор стека

	ASSERT_OK(someStackPtr);

	memset(someStackPtr->stackData, POISON, someStackPtr->stackCapacity);

	free(someStackPtr->stackData);

	someStackPtr = (Stack *)FREED_POINTER;// 13 - маленьное число, маркер уже освобожденных указателей 

	//someStackPtr->stackSize = INT_MAX;//самое большое число size_t какое?
	//someStackPtr->stackCapacity = 0;//норм????  a главное зачем....

	return NO_ERRORS;
}

//надо бы протестить как-то, а так норм
int StackResize (Stack *someStackPtr){//управлет размером памяти, выделенной под стек

	ASSERT_OK(someStackPtr);

	int sizeOfSomeStack     = someStackPtr->stackSize;
	int capacityOfSomeStack = someStackPtr->stackCapacity;
	int *dataOfSomeStack    = someStackPtr->stackData;

	if (capacityOfSomeStack >= LARGE_VOLUME_CRITERION * sizeOfSomeStack){
		int *reallocDataPtr = (int *)realloc(dataOfSomeStack, capacityOfSomeStack * NORMAL_DECREASE_COEFF);

		if (reallocDataPtr == nullptr) {//будет тупо выводить это в отдельную функцию, так как придется проверять что она вернула
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

	ASSERT_OK(someStackPtr);

	return NO_ERRORS;
}

//ну норм
void StackPrint(const Stack someStack){//тут тоже assert_ok?

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

//лучшее
void PrintSeparator(){
	printf("______________________________\n\n");
}

//норм, а что делать с вызовами функций? всё время открывать лог файл и закрывать??
int StackNotOK(const Stack *someStackPtr){// enum-тип ??????
	FILE *logFile = fopen("LIFOlog.txt", "w");

//если у меня size типа size_t, то не делать проверку на >0??
	if (!someStackPtr){
		fprintf(logFile, "Указатель на стек равен нулю\n");
		fclose(logFile);//это что, перед каждым return делать???
		return STK_UNDEFINED;
	}

	if (someStackPtr == (Stack *)FREED_POINTER){//это вообще верно??
		fprintf(logFile, "Указатель на стек уже был удалён!\n");
		fclose(logFile);//это что, перед каждым return делать???
		return TWICE_CALLED_DTOR;
	}

	if (!someStackPtr->stackData){
		fprintf(logFile, "Указатель на данные стека равен нулю\n");
		fclose(logFile);//это что, перед каждым return делать???
		return STK_DATA_UNDEFINED;
	}

	if(someStackPtr->stackSize > someStackPtr->stackCapacity){
		fprintf(logFile, "Размер данных стека больше выделенной под стек памяти\n");
		fclose(logFile);
		return SIZE_LARGER_CAPACITY;
	}

	fclose(logFile);
	return NO_ERRORS;
}


int StackDump_(const Stack *someStackPtr, const int line,	 const char *file,const char *function_name){

	FILE *FileDamp = fopen("LifoDamp.html", "w");


	fprintf(FileDamp, "line = %d, file = %s, function_name = %s", line, file, function_name);


	fclose(FileDamp);
	
	return NO_ERRORS;
}




