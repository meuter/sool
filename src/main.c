#include <stdio.h>
#include <stdlib.h>

#include <sool/io.h>
#include <sool/string.h>

int main() {

	string_t *s = string("hello, this is a test, from the sool lib");
	list_t *l = string_split(s, ", ");

	oprintf("l = %O{%O}\n", l);

	delete(s, l);

	return EXIT_SUCCESS;
}

