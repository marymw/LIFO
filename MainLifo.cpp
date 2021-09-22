#include "Lifo.h"
#include <assert.h>
#include <stdio.h>

int main() {
	Stack FirstStack = {}; //типа инициализация

	int StatusStackCtor = StackCtor(&FirstStack); //конструктор стека, создает стек, возвращает ошибку
	assert(StatusStackCtor == 0);

	int StatusStackPush = StackPush(&FirstStack, 10);//кладет что-то в конец стека
	assert(StatusStackPush == 0);

	int Value = 0;

	int StatusStackPop = StackPop(&FirstStack, &Value); //вытаскивает один элемент из стека
	assert(StatusStackPop == 0);

	printf("%d\n", Value);


	int StatusStackDtor = StackDtor(&FirstStack);//деструктор стека
	assert(StatusStackDtor == 0);

	return 0;
}
