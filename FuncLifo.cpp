#include "Lifo.h"
#include <assert.h>
#include <stdlib.h>
#include <values.h>
#include <string.h>
#include <stdio.h>

int StackCtor(Stack *FirstStackPtr){//конструктор стека, создает стек, возвращает ошибку
	assert(FirstStackPtr); //проверили, существует ли вообще то что нам передали

	int StatusStackCtor = 0; //если никаких ошибок
	int StackCapacity = 10; //пусть в начале выделяется на 10, а там посмотрим

	FirstStackPtr->StackData = (int *)calloc(StackCapacity, sizeof(int));// какой размер передавать стеку в консрукторе

	if (FirstStackPtr->StackData == nullptr){
		StatusStackCtor = 1; //ошибка связанная с выделением памяти
		return StatusStackCtor;
	}

	FirstStackPtr->StackSize = 0; //удалили данные
	//надо какую-то проверку на size потом делать??

	
	return StatusStackCtor; //возвращает код ошибки
}


int StackPush(Stack *FirstStackPtr, int Value){//кладет что-то в конец стека
	assert(FirstStackPtr); //проверили, существует ли вообще то что нам передали

	int StatusStackPush = 0; //если никаких ошибок

	int StatusStackMemory = StackMemory(FirstStackPtr);//функция определяет сколько  памяти зарезервировать стеку
	assert(StatusStackMemory == 0);

	FirstStackPtr->StackData[FirstStackPtr->StackSize] = Value;
	FirstStackPtr->StackSize++;
	
	return StatusStackPush;
}

int StackPop(Stack *FirstStackPtr, int *PtrOnValue){//вытаскивает один элемент из стека
	assert(FirstStackPtr); //проверили, существует ли вообще то что нам передали

	int StatusStackPop = 0; //если никаких ошибок

	int StatusStackMemory = StackMemory(FirstStackPtr);//функция определяет сколько  памяти зарезервировать стеку
	assert(StatusStackMemory == 0);

	FirstStackPtr->StackSize--;
	*PtrOnValue = FirstStackPtr->StackData[FirstStackPtr->StackSize];

	return StatusStackPop;
}

int StackDtor(Stack *FirstStackPtr){//деструктор стека

	assert(FirstStackPtr); //проверили, существует ли вообще то что нам передали

	int StatusStackDtor = 0; //если никаких ошибок

	//залили ядом
	memset(FirstStackPtr->StackData, POISON, FirstStackPtr->StackCapacity); //размер заполняемой области точно StackCapacity или StackSize? почему именно 0xF0
	//так как free не чистит память

	
	free(FirstStackPtr->StackData);
	FirstStackPtr->StackData = (int *)FREED_POINTER;// 13 - маленьное число, маркер уже освобожденных укахателей 

	FirstStackPtr->StackSize = INT_MAX;//самое большое число size_t какое?
	FirstStackPtr->StackCapacity = 0;//норм????


	return StatusStackDtor;
}

int StackMemory (Stack *FirstStackPtr){//управлет размером памяти, выделенной под стек
	int StatusStackMemory = 0; //если всё ок
	printf("Функция %s ещё не реализована\n хрум - хрум\n", __FUNCTION__);
	return StatusStackMemory;
}


//a почему не раюотает???
