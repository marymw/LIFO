#include "Lifo.h"
#include <assert.h>
#include <stdlib.h>
#include <values.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

static int StackResize (Stack *someStackPtr);

static FILE *FileDump = NULL;
static FILE *logFile  = NULL;

const Type 		     POISON          = 666;
const int 		     FREED_POINTER   = 13;
const size_t 	             SIZE_T_MAX      = -1;
const unsigned long long int CANARY_CONST    = 3738381229;
const Type 		     CANARY_FOR_DATA = 100;

int StackCtor(Stack *someStackPtr, size_t someStackCapacity){

	if (someStackPtr == nullptr) {
		return STK_UNDEFINED;
	} 

	logFile  = fopen("LIFOlog.txt", "w");

	if (logFile == NULL) {
		printf("Ошибка открытия файла Log\n");
		return ERROR_IN_OPEN_LOG;
	}

	FileDump = fopen("LifoDump.html", "w");
	
	if (FileDump == NULL){
		printf("Ошибка открытия файла Dump\n");
		return ERROR_IN_OPEN_DUMP;
	}

	someStackPtr->stackCapacity = someStackCapacity; 
	someStackPtr->stackData     = (Type *)calloc(someStackPtr->stackCapacity + 2, sizeof(Type));

	SetPoison(someStackPtr);

	for (int i = 0; i < someStackPtr->stackCapacity + 2; i++){
		someStackPtr->stackData[i] = POISON;
	}
																						
	someStackPtr->stackData += 1;
	someStackPtr->stackSize = 0; 
	someStackPtr->canary1   = CANARY_CONST;
	someStackPtr->canary2   = CANARY_CONST;
	(someStackPtr->stackData)[-1] = CANARY_FOR_DATA;
	(someStackPtr->stackData)[someStackPtr->stackCapacity] = CANARY_FOR_DATA;
	
	someStackPtr->hash = MyHashRot13((const char *)someStackPtr);

	ASSERT_OK(someStackPtr);

	return NO_ERRORS; 
}


int StackPush(Stack *someStackPtr, const Type value){ 

	ASSERT_OK(someStackPtr);

	int statusStackMemory = StackResize(someStackPtr);
	if (statusStackMemory != 0){
		return statusStackMemory;
	}

	someStackPtr->stackData[someStackPtr->stackSize] = value;
	someStackPtr->stackSize++;

	someStackPtr->hash = MyHashRot13((const char *)someStackPtr);
	ASSERT_OK(someStackPtr);
	
	return NO_ERRORS;
}


Type StackPop(Stack *someStackPtr, int *statusStackPop){

	ASSERT_OK(someStackPtr);

	int statusStackMemory = StackResize(someStackPtr);
	if (statusStackMemory != 0){
		return statusStackMemory;
	}

	someStackPtr->stackSize--;
	size_t sizeOfSomeStack = someStackPtr->stackSize;

	Type valueFromPop = someStackPtr->stackData[sizeOfSomeStack];

	someStackPtr->stackData[sizeOfSomeStack] = POISON;

	someStackPtr->hash = MyHashRot13((const char *)someStackPtr);
	ASSERT_OK(someStackPtr);

	return valueFromPop;
}


int StackDtor(Stack *someStackPtr){

	ASSERT_OK(someStackPtr);

        memset((someStackPtr->stackData - 1), POISON, someStackPtr->stackCapacity + 2);

	free(someStackPtr->stackData - 1);
	
	someStackPtr->stackSize     = SIZE_T_MAX;
	someStackPtr->stackCapacity = 0;
	someStackPtr->canary1	    = 0;
	someStackPtr->canary2 	    = 0;
	someStackPtr->hash 	    = 0;

	fclose(FileDump);
	fclose(logFile);

	return NO_ERRORS;
}


static int StackResize(Stack *someStackPtr){

	ASSERT_OK(someStackPtr);

	int   sizeOfSomeStack     = someStackPtr->stackSize;
	int   capacityOfSomeStack = someStackPtr->stackCapacity;
	Type *dataOfSomeStack     = someStackPtr->stackData - 1;

	if (capacityOfSomeStack >= LARGE_VOLUME_CRITERION * sizeOfSomeStack){

		Type *reallocDataPtr = (Type *)realloc(dataOfSomeStack, capacityOfSomeStack / NORMAL_DECREASE_COEFF + 2);

		if (reallocDataPtr == nullptr) {
			return LACK_OF_MEMORY;
		}

		someStackPtr->stackData       = reallocDataPtr + 1;
		someStackPtr->stackCapacity  /= NORMAL_DECREASE_COEFF;
		(someStackPtr->stackData)[-1] = CANARY_FOR_DATA;
		(someStackPtr->stackData)[someStackPtr->stackCapacity] = CANARY_FOR_DATA;
	
	}
	else if (capacityOfSomeStack <= SMALL_VOLUME_CRITERION * sizeOfSomeStack){

		if (sizeOfSomeStack > BIG_SIZE_OF_STACK) {
			
			Type *reallocDataPtr = (Type *)realloc(dataOfSomeStack, capacityOfSomeStack * SMALL_INCREASE_COEFF + 2);

			if (reallocDataPtr == nullptr) {
				return LACK_OF_MEMORY;
			}

			someStackPtr->stackData       = reallocDataPtr + 1;
			someStackPtr->stackCapacity  *= SMALL_INCREASE_COEFF;
			(someStackPtr->stackData)[-1] = CANARY_FOR_DATA;
			(someStackPtr->stackData)[someStackPtr->stackCapacity] = CANARY_FOR_DATA;

		}
		else {
			Type *reallocDataPtr = (Type *)realloc(dataOfSomeStack, capacityOfSomeStack * NORMAL_INCREASE_COEFF + 2);

			if (reallocDataPtr == nullptr) {
				return LACK_OF_MEMORY;
			}

			someStackPtr->stackData       = reallocDataPtr + 1;
			someStackPtr->stackCapacity  *= NORMAL_INCREASE_COEFF; 
			(someStackPtr->stackData)[-1] = CANARY_FOR_DATA;
			(someStackPtr->stackData)[someStackPtr->stackCapacity] = CANARY_FOR_DATA;

		}
	}
	else{
		;
	}
	
	someStackPtr->hash = MyHashRot13((const char *)someStackPtr);
	ASSERT_OK(someStackPtr);

	return NO_ERRORS;
}


void IntStackPrint(const Stack* someStackPtr){//тут тоже assert_ok?

	PrintSeparator();
	printf("Printing stack...\n\n");

	if (someStackPtr->stackSize == 0) {
		printf("Stack is empty! :)\n\n");
		PrintSeparator();
		return;
	}

	for (int i = 0; i < someStackPtr->stackSize; i++){
		printf("%d\n", someStackPtr->stackData[i]);
	}

	PrintSeparator();

	return;
}


void PrintSeparator(){
	printf("______________________________\n\n");
}


int StackNotOK(const Stack *someStackPtr, const int line, const char *file, const char *function_name){

	fprintf(logFile, "В файле %s на строчке %d вызвана функция %s.\n", file, line, function_name);

	NullPtrCheck			  (someStackPtr, logFile, "Указатель на стек равен нулю\n", STK_UNDEFINED);
	FreedPtrCheck			  (someStackPtr, FREED_POINTER, logFile, "Указатель на стек уже был удалён!\n", TWICE_CALLED_DTOR);
	NullPtrCheck			  (someStackPtr->stackData, logFile, "Указатель на данные стека равен нулю\n", STK_DATA_UNDEFINED);
	CompOsSizeAndCapacityCheck((someStackPtr->stackSize), (someStackPtr->stackCapacity), logFile, "Размер данных стека больше выделенной под стек памяти\n", SIZE_LARGER_CAPACITY);
	CanaryCheck 			  ((someStackPtr->canary1), CANARY_CONST, logFile, "Несовпадение левой стековой канарейки!! Её значение canary1 =  %x \n", CANARY_MISMATCH); 
	CanaryCheck 			  ((someStackPtr->canary2), CANARY_CONST, logFile, "Несовпадение правой стековой канарейки!! Её значение canary2 =  %x \n", CANARY_MISMATCH);	
	CanaryCheck 			  (((someStackPtr->stackData)[-1]), CANARY_FOR_DATA, logFile, "Несовпадение левой канарейки данных!! Её значение canary3 =  %x \n", CANARY_MISMATCH);																		
	CanaryCheck 			  ((((someStackPtr->stackData)[someStackPtr->stackCapacity])), CANARY_FOR_DATA, logFile, "Несовпадение правой канарейки данных!! Её значение canary4 =  %x \n", CANARY_MISMATCH);
	
	unsigned int hashOfStack = MyHashRot13((const char *)someStackPtr);
	HashCheck(hashOfStack, (someStackPtr->hash), logFile, "Несовпадение хешей! Нынешнее значение = %u, ожидаемое значение = %u\n", HASH_MISMATCH);

	return NO_ERRORS;
}


int StackDump_(const Stack *someStackPtr, const int line, const char *file, const char *function_name){

	int statusStackNotOK = StackNotOK(someStackPtr, __LINE__, __FILE__, __FUNCTION__);

	fprintf(FileDump, "<pre>Вызван для типа: Stack , c типом элементов: \\Type\\, адрес стековой переменой : %p, верификатор выдал %d, name : called from function  %s, at file %s(%d)\n", someStackPtr, statusStackNotOK, function_name, file, line);

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

	return NO_ERRORS;
}

void PrintElement(const Stack *someStackPtr){
	
	for (int i = -1; i < (int)(someStackPtr->stackCapacity) + 1; i++){
		
		if (i < (int)someStackPtr->stackSize){
			fprintf(FileDump, "*[%d] = %lf\n\n", i, someStackPtr->stackData[i]);
		}
		else {
			fprintf(FileDump, "[%d] = %lf\n\n", i, someStackPtr->stackData[i]);
		}
	}
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

Type StackTop(Stack *someStackPtr){

	ASSERT_OK(someStackPtr);

	size_t size = someStackPtr->stackSize - 1;

	Type valueFromTop = someStackPtr->stackData[size];

	ASSERT_OK(someStackPtr);

	return valueFromTop;
}


hash_t MyHashRot13(const char * str){

    unsigned int hash = 0;

    for(int i = 0; i < 32 ; i++,  str++)//40 - размер стека без хеша
    {
        hash += (unsigned char)(*str);
        hash -= (hash << 13) | (hash >> 19);
    }

    return hash;

}

int SetPoison(Stack *someStackPtr){

	if (someStackPtr == nullptr) 
		return STK_UNDEFINED;
	if (someStackPtr->stackData == nullptr)
		return STK_DATA_UNDEFINED;

	for (int i = 0; i < someStackPtr->stackCapacity + 2; i++){
		someStackPtr->stackData[i] = (Type)POISON;
	}

	return NO_ERRORS;
}
