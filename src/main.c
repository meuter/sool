#include <stdio.h>
#include <stdlib.h>

#include <cobject/io.h>
#include <cobject/list.h>

int main() {
	list_t *l1 = list(1,2,3,4,5,6,7);
	list_t *l2 = list("toto", "boo", "bar");
	list_t *l3 = list();
	item_t *i;

	int x = oprintf("%O{%d}\n", l1);
	int y = oprintf("%O{%s}\n", l2);
	int z = oprintf("%O\n", l3);

	for (i = list_begin(l2); i != list_end(l2); i = item_next(i))
		printf("%s\n", (char *)item_get(i));


	for (i = list_rbegin(l1); i != list_rend(l1); i = item_previous(i)) {
		int x = (int)item_get(i);
		printf("%d ", x);
	}

	list_forall(i, l2) {
		printf("--> %s\n", (char *)item_get(i));
	}

	printf("x = %d / y = %d / z = %d\n", x, y, z);
	delete(l1, l2, l3);
	return EXIT_SUCCESS;
}

