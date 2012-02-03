#include <stdio.h>
#include <stdlib.h>
#include <sool/exception.h>
#include <sool/stack.h>
#include <sool/io.h>

int toto(bool_t error) {
	printf("entering toto\n");
	if (error)
		throw((void*)15156);
	printf("leaving toto\n");
	return 0;
}


int main() {

	int x;

	try {
		printf("before toto \n");
		int y = toto(FALSE);
		printf("after toto 1 : y = %d\n", y);
		int x = toto(TRUE);
		printf("after toto 2 : x = %d\n", x);
	}
	else if ( __top() == (void*)1515 && ( x = (int)__catch())) {
		printf("caught 1515\n");
	}
	else if ( __top() == (void*)15156 && ( x = (int)__catch())) {
		printf("caught 15156\n");
	}
	else {
		printf("caught something else\n");
	}




	return 0;
}

