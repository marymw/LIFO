#include "Lifo.h"
#include <assert.h>
#include <stdlib.h>
#include <values.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

//type
//норм
int StackCtor(Stack *someStackPtr){

	if (someStackPtr == 0) {
		return StackNotOK(someStackPtr, __LINE__, __FILE__, "StackCtor");//ну такое себе решение..
	} 

	someStackPtr->stackCapacity = 10; //пусть в начале выделяется на 10, а там посмотрим
	someStackPtr->stackData     = (int *)calloc(someStackPtr->stackCapacity, sizeof(int));
	memset(someStackPtr->stackData, POISON, someStackPtr->stackCapacity);//что не тааааааак
																						
	if (someStackPtr->stackData == nullptr){ 
		return StackNotOK(someStackPtr, __LINE__, __FILE__, "StackCtor");
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
int StackNotOK(const Stack *someStackPtr, const int line, const char *file, const char *function_name){// enum-тип ??????
	FILE *logFile = fopen("LIFOlog.txt", "a");//а как сделать чтобы в чистый файл писала?

	fprintf(logFile, "В файле %s на строчке %d вызвана функция %s.\n", file, line, function_name);

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


int StackDump_(const Stack *someStackPtr, const int line, const char *file, const char *function_name){
//а здесь с режимом a или w лучше открыть??
	FILE *FileDump = fopen("LifoDamp.html", "w");

	int statusStackNotOK = StackNotOK(someStackPtr, __LINE__, __FILE__, __FUNCTION__);

	fprintf(FileDump, "<pre>Вызван для типа:  , c типом элементов: , адрес стековой переменой : %p, верификатор выдал %d, name : called from function  %s, at file %s(%d)\n", someStackPtr, statusStackNotOK, function_name, file, line);

	if(someStackPtr == 0)
		fprintf(FileDump, "Указатель на стек равен нулю, дальше печатать ничего не буду!\n");

	else {
		fprintf(FileDump, "size = %zu\n capacity = %zu\n", someStackPtr->stackSize, someStackPtr->stackCapacity);
		if (someStackPtr->stackData == 0){
			fprintf(FileDump, "нулевой указатель на data. Дальше печатать ничего не буду\n");
		}
		else{
			fprintf(FileDump, "data[%p]\n", someStackPtr->stackData);
			PrintElement(someStackPtr);
		}
	}

	fprintf(FileDump, "Тут мои полномочия всё\n</pre>");
	fclose(FileDump);

	return NO_ERRORS;
}

void PrintElement(const Stack *someStackPtr){

FILE *FileDump = fopen("LifoDamp.html", "a");
fprintf(FileDump, "<pre>");
	for (int i = 0; i < someStackPtr->stackCapacity; i++){
		if (i < someStackPtr->stackSize){
			fprintf(FileDump, "*[%d] = %d\n\n", i, someStackPtr->stackData[i]);
		}
		else {
			fprintf(FileDump, "[%d] = %d\n\n", i, someStackPtr->stackData[i]);
		}

	}
	fprintf(FileDump, "</pre>");
}

void MyMemcpy(void *newObject, const void *oldObject, size_t numberOfSymbols){


	int i = 0;
	for (; i < (numberOfSymbols / sizeof(long long int)); i++ ){
			
		((long long int *)newObject)[i] = ((long long int *)oldObject)[i];

	}

	numberOfSymbols = numberOfSymbols % sizeof(long long int);

	for (; i < (numberOfSymbols / sizeof(int)); i++ ){
		
		((int *)newObject)[i] = ((int *)oldObject)[i];

	}

	numberOfSymbols = numberOfSymbols % sizeof(int);

	for (; i < (numberOfSymbols / sizeof(short int)); i++ ){
		
		((short int *)newObject)[i] = ((short int *)oldObject)[i];

	}

	numberOfSymbols = numberOfSymbols % sizeof(short int);

	for (; i < (numberOfSymbols / sizeof(char)); i++ ){
		
		((char *)newObject)[i] = ((char *)oldObject)[i];

	}
		
}
