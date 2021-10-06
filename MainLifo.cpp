#include "Lifo.h"
#include <assert.h>
#include <stdio.h>


int main() {
	Stack firstStack = {}; 

	int statusStackCtor = StackCtor(&firstStack, 10); 
	assert(statusStackCtor == 0);

	//StackDump(&firstStack);

	int statusStackPush = StackPush(&firstStack, 10.5);//кладет что-то в конец стека
	assert(statusStackPush == 0);

	//StackDump(&firstStack);
	Type valueStackTop = StackTop (&firstStack);
	printf("%lf\n", valueStackTop);

	statusStackPush = StackPush(&firstStack, 20);//кладет что-то в конец стека
	assert(statusStackPush == 0);

	valueStackTop = StackTop (&firstStack);
	printf("%lf\n", valueStackTop);

	StackDump(&firstStack);
	int statusStackPop = 0;

	Type valueFromPop1 = StackPop(&firstStack, &statusStackPop); //вытаскивает один элемент из стека
	assert(statusStackPop == 0);

	printf("%lf\n", valueFromPop1);

	int statusStackDtor = StackDtor(&firstStack);//деструктор стека
	assert(statusStackDtor == 0);


	return 0;
}
