#include "Lifo.h"
#include <assert.h>
#include <stdio.h>


int main() {
	Stack firstStack = {}; 

	int statusStackCtor = StackCtor(&firstStack, 10); 
	assert(statusStackCtor == 0);

	//StackDump(&firstStack);

	int statusStackPush = StackPush(&firstStack, 10);//кладет что-то в конец стека
	assert(statusStackPush == 0);

	int valueStackTop = StackTop (firstStack);
	printf("%d\n", valueStackTop);

	statusStackPush = StackPush(&firstStack, 20);//кладет что-то в конец стека
	assert(statusStackPush == 0);

	StackDump(&firstStack);
	int statusStackPop = 0;

	int valueFromPop1 = StackPop(&firstStack, &statusStackPop); //вытаскивает один элемент из стека
	assert(statusStackPop == 0);

	int statusStackDtor = StackDtor(&firstStack);//деструктор стека
	assert(statusStackDtor == 0);

	return 0;
}
