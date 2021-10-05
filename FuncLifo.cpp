#include "Lifo.h"
#include <assert.h>
#include <stdlib.h>
#include <values.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

static int StackResize (Stack *someStackPtr);

const Type 					 POISON          = 66;
const int 					 FREED_POINTER   = 13;
const size_t 				 SIZE_T_MAX      = -1;
const unsigned long long int CANARY_CONST    = 3738381229;
const Type 					 CANARY_FOR_DATA = 100;

int StackCtor(Stack *someStackPtr, size_t someStackCapacity){

	if (someStackPtr == nullptr) {
		return STK_UNDEFINED;
	} 

	someStackPtr->stackCapacity = someStackCapacity; 
	someStackPtr->stackData     = (Type *)calloc(someStackPtr->stackCapacity + 2, sizeof(Type));//+1 для канарейки сделать??

	if (someStackPtr->stackData == nullptr){ 
		return StackNotOK(someStackPtr, __LINE__, __FILE__, "StackCtor");
	}

	memset(someStackPtr->stackData, 0xF0, someStackPtr->stackCapacity);//нельзя???
																						
	someStackPtr->stackData++;
	someStackPtr->stackSize = 0; 
	someStackPtr->canary1 = CANARY_CONST;
	someStackPtr->canary2 = CANARY_CONST;
	(someStackPtr->stackData)[-1] = CANARY_FOR_DATA;
	(someStackPtr->stackData)[someStackPtr->stackCapacity] = CANARY_FOR_DATA;
	
	someStackPtr->hash = MyHashRot13((const char *)someStackPtr); 
	ASSERT_OK(someStackPtr);

	return NO_ERRORS; 
}

//норм
int StackPush(Stack *someStackPtr, const Type value){ 

	ASSERT_OK(someStackPtr);

	int statusStackMemory = StackResize(someStackPtr);
	assert(statusStackMemory == 0);//надо чем-то заменить?

	someStackPtr->stackData[someStackPtr->stackSize] = value;
	someStackPtr->stackSize++;

	someStackPtr->hash = MyHashRot13((const char *)someStackPtr);
	ASSERT_OK(someStackPtr);
	
	return NO_ERRORS;
}

//норм
Type StackPop(Stack *someStackPtr, int *statusStackPop){

	ASSERT_OK(someStackPtr);

	int statusStackMemory = StackResize(someStackPtr);
	assert(statusStackMemory == 0);//в лог, не падать

	someStackPtr->stackSize--;
	size_t sizeOfSomeStack = someStackPtr->stackSize;//int sizeOfSomeStack = (--someStackPtr->stackSize);

	Type valueFromPop = someStackPtr->stackData[sizeOfSomeStack];

	someStackPtr->stackData[sizeOfSomeStack] = POISON;

	someStackPtr->hash = MyHashRot13((const char *)someStackPtr);
	ASSERT_OK(someStackPtr);

	return valueFromPop;
}

//канарейки удалять?
int StackDtor(Stack *someStackPtr){//деструктор стека

	ASSERT_OK(someStackPtr);
//memset(someStackPtr->stackData - 1, 0xF0, someStackPtr->stackCapacity + 2);//еще для канареек
	memset(someStackPtr->stackData, 0xF0, someStackPtr->stackCapacity);

	free(someStackPtr->stackData - 1);
	
	someStackPtr->stackSize = SIZE_T_MAX;
	someStackPtr->stackCapacity = 0;

	return NO_ERRORS;
}


static int StackResize (Stack *someStackPtr){//управлет размером памяти, выделенной под стек

	ASSERT_OK(someStackPtr);

	int   sizeOfSomeStack     = someStackPtr->stackSize;
	int   capacityOfSomeStack = someStackPtr->stackCapacity;
	Type *dataOfSomeStack     = someStackPtr->stackData - 1;

	if (capacityOfSomeStack >= LARGE_VOLUME_CRITERION * sizeOfSomeStack){
		Type *reallocDataPtr = (Type *)realloc(dataOfSomeStack, capacityOfSomeStack * NORMAL_DECREASE_COEFF + 2);

		if (reallocDataPtr == nullptr) {//будет тупо выводить это в отдельную функцию, так как придется проверять что она вернула
			return LACK_OF_MEMORY;
		}

		someStackPtr->stackData       = reallocDataPtr + 1;
		someStackPtr->stackCapacity  *= NORMAL_DECREASE_COEFF;
		(someStackPtr->stackData)[-1] = CANARY_FOR_DATA;
		(someStackPtr->stackData)[someStackPtr->stackCapacity] = CANARY_FOR_DATA;
		printf(" значение canary4 =  %d \n", (someStackPtr->stackData)[someStackPtr->stackCapacity]);
		printf("Значение CANARY_FOR_DATA = %d , %x \n", CANARY_FOR_DATA, CANARY_FOR_DATA);
		printf(" Указатель на данные %p\n", someStackPtr->stackData + someStackPtr->stackCapacity );
	
	}
	else if (capacityOfSomeStack <= SMALL_VOLUME_CRITERION * sizeOfSomeStack){

		if (sizeOfSomeStack > BIG_SIZE_OF_STACK) {
			//a realloc чистит память??
			Type *reallocDataPtr = (Type *)realloc(dataOfSomeStack, capacityOfSomeStack * SMALL_INCREASE_COEFF + 2);

			if (reallocDataPtr == nullptr) {
				return LACK_OF_MEMORY;
			}

			someStackPtr->stackData      = reallocDataPtr + 1;
			someStackPtr->stackCapacity *= SMALL_INCREASE_COEFF;
			(someStackPtr->stackData)[-1] = CANARY_FOR_DATA;
			(someStackPtr->stackData)[someStackPtr->stackCapacity] = CANARY_FOR_DATA;
			printf(" значение canary4 =  %d \n", (someStackPtr->stackData)[someStackPtr->stackCapacity]);
			printf("Значение CANARY_FOR_DATA = %d , %x \n", CANARY_FOR_DATA, CANARY_FOR_DATA);
			printf(" Указатель на данные %p\n", someStackPtr->stackData + someStackPtr->stackCapacity );
		}
		else {
			Type *reallocDataPtr = (Type *)realloc(dataOfSomeStack, capacityOfSomeStack * NORMAL_INCREASE_COEFF + 2);

			if (reallocDataPtr == nullptr) {
				return LACK_OF_MEMORY;
			}

			someStackPtr->stackData = reallocDataPtr + 1;
			someStackPtr->stackCapacity *= NORMAL_INCREASE_COEFF; 
			(someStackPtr->stackData)[-1] = CANARY_FOR_DATA;
			(someStackPtr->stackData)[someStackPtr->stackCapacity] = CANARY_FOR_DATA;
			printf(" значение canary4 =  %d \n", (someStackPtr->stackData)[someStackPtr->stackCapacity]);
			printf("Значение CANARY_FOR_DATA = %d , %x \n", CANARY_FOR_DATA, CANARY_FOR_DATA);
			printf(" Указатель на данные %p\n", someStackPtr->stackData + someStackPtr->stackCapacity );

		}
	}

	

	someStackPtr->hash = MyHashRot13((const char *)someStackPtr);
	ASSERT_OK(someStackPtr);

	return NO_ERRORS;
}

//ну норм
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

//лучшее
void PrintSeparator(){
	printf("______________________________\n\n");
}

int StackNotOK(const Stack *someStackPtr, const int line, const char *file, const char *function_name){// enum-тип ??????
	FILE *logFile = fopen("LIFOlog.txt", "a");

	fprintf(logFile, "В файле %s на строчке %d вызвана функция %s.\n", file, line, function_name);

	NullPtrCheck(someStackPtr, logFile, "Указатель на стек равен нулю\n", STK_UNDEFINED);

	FreedPtrCheck(someStackPtr, FREED_POINTER, logFile, "Указатель на стек уже был удалён!\n", TWICE_CALLED_DTOR);

	NullPtrCheck(someStackPtr->stackData, logFile, "Указатель на данные стека равен нулю\n", STK_DATA_UNDEFINED);

	if(someStackPtr->stackSize > someStackPtr->stackCapacity){
		fprintf(logFile, "Размер данных стека больше выделенной под стек памяти\n");
		fclose(logFile);
		return SIZE_LARGER_CAPACITY;
	}

	CanaryCheck ((someStackPtr->canary1), CANARY_CONST, logFile, "Несовпадение левой стековой канарейки!! Её значение canary1 =  %x \n", CANARY_MISMATCH); 
	CanaryCheck ((someStackPtr->canary2), CANARY_CONST, logFile, "Несовпадение правой стековой канарейки!! Её значение canary2 =  %x \n", CANARY_MISMATCH);	
	CanaryCheck (((someStackPtr->stackData)[-1]), CANARY_FOR_DATA, logFile, "Несовпадение левой канарейки данных!! Её значение canary3 =  %x \n", CANARY_MISMATCH);
																			
/*
	if ((someStackPtr->stackData)[someStackPtr->stackCapacity] != CANARY_FOR_DATA){
		fprintf(logFile, "Несовпадение правой канарейки данных!! Её значение canary4 =  %d \n", (someStackPtr->stackData)[someStackPtr->stackCapacity]);
		fprintf(logFile, "Значение CANARY_FOR_DATA = %d , %x \n", CANARY_FOR_DATA, CANARY_FOR_DATA);
		fprintf(logFile, " Указатель на данные %p\n", someStackPtr->stackData + someStackPtr->stackCapacity );
		fclose(logFile);
		return CANARY_MISMATCH;
	}

*/	
	unsigned int hashOfStack = MyHashRot13((const char *)someStackPtr);
	HashCheck(hashOfStack, (someStackPtr->hash), logFile, "Несовпадение хешей! Нынешнее значение = %u, ожидаемое значение = %u\n", HASH_MISMATCH);

	fclose(logFile);
	return NO_ERRORS;
}


int StackDump_(const Stack *someStackPtr, const int line, const char *file, const char *function_name){
//а здесь с режимом a или w лучше открыть??
	FILE *FileDump = fopen("LifoDump.html", "a");

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

FILE *FileDump = fopen("LifoDump.html", "a");
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

Type StackTop(Stack someStack){

	ASSERT_OK(&someStack);

	size_t size = someStack.stackSize - 1;

	Type valueFromTop = someStack.stackData[size];

	ASSERT_OK(&someStack);

	return valueFromTop;
}
/*
unsigned int MyHashRot13(const char * str)
{

    unsigned int hash = 0;

    for(; *str ; str++)
    {
        hash += (unsigned char)(*str);
        hash -= (hash << 13) | (hash >> 19);
    }

    return hash;

}
*/

hash_t MyHashRot13(const char * str){

    unsigned int hash = 0;

    for(int i = 0; i < 32 ; i++,  str++)//40 - размер стека без хеша
    {
        hash += (unsigned char)(*str);
        hash -= (hash << 13) | (hash >> 19);
    }

    return hash;

}
