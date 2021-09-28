#include "Lifo.h"
#include <assert.h>
#include <stdio.h>

int main() {
	Stack firstStack = {}; //типа инициализация

	int statusStackCtor = StackCtor(&firstStack); //конструктор стека, создает стек, возвращает ошибку
	assert(statusStackCtor == 0);

	int statusStackPush = StackPush(&firstStack, 10);//кладет что-то в конец стека
	assert(statusStackPush == 0);

	statusStackPush = StackPush(&firstStack, 20);//кладет что-то в конец стека
	assert(statusStackPush == 0);

	int statusStackPop = 0;

	int valueFromPop1 = StackPop(&firstStack, &statusStackPop); //вытаскивает один элемент из стека
	assert(statusStackPop == 0);

	int valueFromPop2 = StackPop(&firstStack); //вытаскивает один элемент из стека
	assert(statusStackPop == 0);

	int statusStackDtor = StackDtor(&firstStack);//деструктор стека
	assert(statusStackDtor == 0);

	return 0;
}
