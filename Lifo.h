#ifndef LIFO_H
#define LIFO_H

#include <string.h>
#include <stdio.h>

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
	CANARY_MISMATCH,     //6
	HASH_MISMATCH		 //7
};

enum Constants{
	LARGE_VOLUME_CRITERION = 8,
	SMALL_VOLUME_CRITERION = 2,
	NORMAL_DECREASE_COEFF = 2,  
	BIG_SIZE_OF_STACK = 100,		
	NORMAL_INCREASE_COEFF = 3,  
	SMALL_INCREASE_COEFF = 2	
};

typedef int 				   Type;
typedef unsigned long long int canary_t;
typedef unsigned int 		   hash_t;

extern const Type 					POISON;
extern const Type 					CANARY_FOR_DATA;
extern const int 					FREED_POINTER;
extern const size_t 				SIZE_T_MAX;
extern const unsigned long long int CANARY_CONST;

#define NullPtrCheck(someStackPtr, logFile, string, nameOfError) do {							 \
																	if (!someStackPtr){		     \
																		fprintf(logFile, string);\
																		return(nameOfError);     \
																	}	 						 \
																 } while(0)


#define FreedPtrCheck(someStackPtr, nameOfConst, logFile, string, nameOfError) do {							  \
																	if(someStackPtr == (Stack *)nameOfConst){ \
																		fprintf(logFile, string);			  \
																		return(nameOfError);				  \
																	}	 				 					  \
																		       } while(0)


#define CanaryCheck(canary, canaryConst, logFile, string, nameOfError) do {									     \
																			if (canary != canaryConst){ 		 \
																				fprintf(logFile, string, canary);\
																				return nameOfError;			     \
																			}									 \
																		} while(0)

#define HashCheck(hashValue, hashExpectedValue, logFile, string, nameOfError) do {								   \
														if (hashValue != hashExpectedValue){ 					   \
															fprintf(logFile, string, hashValue, hashExpectedValue);\
															return nameOfError;			     					   \
														}									 					   \
																		     } while(0)


#define CompOsSizeAndCapacityCheck(size, capacity, logFile, string, nameOfError) do {								\
																		    if(size > capacity){					\
																				fprintf(logFile, string);			\
																				return nameOfError;					\
																			}										\
																		 }while(0)


struct Stack{
	
	canary_t canary1;

	Type  	*stackData;
	size_t   stackSize;
	size_t   stackCapacity; 

	hash_t   hash;

	canary_t canary2;
};

#define ASSERT_OK(someStackPtr) do {																 \
									if((StackNotOK(someStackPtr, __LINE__, __FILE__, __FUNCTION__))){\
 										StackDump(someStackPtr);									 \
 										assert((!"Stack OK"));}										 \
						        } while (0)																													


#define StackDump(someStackPtr) do { 																\
									StackDump_(someStackPtr, __LINE__, __FILE__, __FUNCTION__);		\
								} while (0)															


int    StackDtor     (Stack *someStackPtr);
int    StackCtor     (Stack *someStackPtr, size_t someStackCapacity);
int    SetPoison     (Stack *someStackPtr);
Type   StackTop      (Stack someStack);
int    StackPush     (Stack *firstStackPtr, Type value);
Type   StackPop      (Stack *someStackPtr, int *statusStackPop = NULL);
int    StackDump_    (const Stack *someStackPtr, const int line, const char *file, const char *function_name);
int    StackNotOK    (const Stack *someStackPtr, const int line, const char *file, const char *function_name);
void   IntStackPrint (Stack someStack);
void   PrintElement  (const Stack *someStackPtr);
void   PrintSeparator();
void   MyMemcpy      (void *newObject, const void *oldObject, size_t numberOfSymbols);
hash_t MyHashRot13   (const char * str);

#endif LIFO_H
