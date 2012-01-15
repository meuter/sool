#include <stdio.h>
#include <stdlib.h>

#include <cobject/io.h>
#include <cobject/list.h>


#include <cobject/args.h>

int main() {

	list_t *l1 = list(1,2,3,4,5,6,7);
	list_t *l2 = list("toto", "boo", "bar");
	list_t *l3 = list();

	int x = oprintf("%O{%d}\n", l1);
	int y = oprintf("%O{%s}\n", l2);
	int z = oprintf("%O\n", l3);

#if NEEDED
	iterator_t *i;
	for (i = list_begin(l1); i != _list_end(l1); ++i)
		printf("%d\n", iterator_get(i);
#endif


	printf("x = %d / y = %d / z = %d\n", x, y, z);
	delete(l1);
	delete(l2);
	delete(l3);
	return EXIT_SUCCESS;
}

