#include <stdio.h>
#include <stdlib.h>
#include <sool/list.h>
#include <sool/io.h>


int main() {
	list_t *l = list(1,2,3,4,5,6);
	iterator_t *i;

	forall(i,l) 
		oprintf("%O -> %d\n", i, (int)get(i));

	return EXIT_SUCCESS;
}

