#include <stdio.h>
#include <stdlib.h>

#include <cobject/class.h>
#include <cobject/io.h>
#include <cobject/list.h>


int main() {
	void *l = list(1,2,3,4,5,6,7);
	int x = oprintf("%O{%d}\n", l);

	printf("%d\n", x);
	delete(l);
	return EXIT_SUCCESS;
}

