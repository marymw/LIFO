#include "Lifo.h"
#include <assert.h>
#include <stdio.h>

int main() {
	Stack FirstStack = {}; //типа инициализация

	int StatusStackCtor = StackCtor(&FirstStack); //конструктор стека, создает стек, возвращает ошибку
	assert(StatusStackCtor == 0);

	int StatusStackPush = StackPush(&FirstStack, 10);//кладет что-то в конец стека
	assert(StatusStackPush == 0);

	StatusStackPush = StackPush(&FirstStack, 20);//кладет что-то в конец стека
	assert(StatusStackPush == 0);

	int StatusStackPop = 0;

	int ValueFromPop1 = StackPop(&FirstStack, &StatusStackPop); //вытаскивает один элемент из стека
	assert(StatusStackPop == 0);

	printf("%d\n", ValueFromPop1);

	int ValueFromPop2 = StackPop(&FirstStack); //вытаскивает один элемент из стека
	assert(StatusStackPop == 0);

	printf("%d\n", ValueFromPop2);


	int StatusStackDtor = StackDtor(&FirstStack);//деструктор стека
	assert(StatusStackDtor == 0);

	return 0;
}
