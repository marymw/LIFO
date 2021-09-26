#include "Lifo.h"
#include <assert.h>
#include <stdio.h>

int main() {
	Stack FirstStack = {}; //типа инициализация

	int StatusStackCtor = StackCtor(&FirstStack); //конструктор стека, создает стек, возвращает ошибку
	assert(StatusStackCtor == 0);

	StackPrint(FirstStack);


	int StatusStackPush = StackPush(&FirstStack, 10);//кладет что-то в конец стека
	assert(StatusStackPush == 0);

	StackPrint(FirstStack);


	StatusStackPush = StackPush(&FirstStack, 20);//кладет что-то в конец стека
	assert(StatusStackPush == 0);

	StackPrint(FirstStack);


	int StatusStackPop = 0;

	StackPrint(FirstStack);

	int ValueFromPop1 = StackPop(&FirstStack, &StatusStackPop); //вытаскивает один элемент из стека
	assert(StatusStackPop == 0);

	printf("%d\n", ValueFromPop1);

	StackPrint(FirstStack);

	int ValueFromPop2 = StackPop(&FirstStack); //вытаскивает один элемент из стека
	assert(StatusStackPop == 0);

	printf("%d\n", ValueFromPop2);

	StackPrint(FirstStack); 

	for (int i = 0; i < 25; i++) {//на 50 уже падает

		StatusStackPush = StackPush(&FirstStack, 10);//кладет что-то в конец стека
		assert(StatusStackPush == 0);

	}

	StackPrint(FirstStack);

	int StatusStackDtor = StackDtor(&FirstStack);//деструктор стека
	assert(StatusStackDtor == 0);

	return 0;
}
