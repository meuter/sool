#include <stdio.h>
#include <stdlib.h>
#include <sool/exception.h>
#include <sool/stack.h>
#include <sool/io.h>

int some_function(int error) {
	printf("entering...\n");
	if (error == 1)
		throw(new(Exception(), "something was thrown"));
	else if (error == 2)
		throw(new(Object()));
	else if (error == 3)
		throw(new(Stack()));
	printf("leaving...\n");
	return 0;
}


int main() {

	void *o;

	try {
		printf("step 1\n");
		some_function(0);
		printf("step 2\n");
		some_function(3);
		printf("step 3\n");
	}
	catch(Exception(), o) {
		oprintf("caught exception [%O]\n", o);
	}
	catch(Object(), o) {
		oprintf("caught an object [%O]\n", o);
	}
	except {
		oprintf("caught something else\n");
	}

	printf("out of the danger zone...\n");

	throw(new(Exception(), "fatal mistake!"));

	printf("this will never be printed!\n");

	return 0;
}

