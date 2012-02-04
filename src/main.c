#include <stdio.h>
#include <stdlib.h>
#include <sool/exception.h>
#include <sool/stack.h>
#include <sool/io.h>

int toto(int error) {
	printf("entering toto\n");
	if (error == 1)
		throw(new(Exception(), "youpla"));
	else if (error == 2)
		throw(new(Object()));
	else
		throw(new(Stack()));
	printf("leaving toto\n");
	return 0;
}


int main() {

	void *e;

	try {
		printf("before toto \n");
		int y = toto(0);
		printf("after toto 1 : y = %d\n", y);
		int x = toto(1);
		printf("after toto 2 : x = %d\n", x);
	}
	catch(Exception(), e) {
		oprintf("caught exception %O\n", e);
	}
	catch(Object(), e) {
		oprintf("caught toto exception %O\n", e);
	}
	other(e) {
		oprintf("caught something else\n");
	}

	return 0;
}

