#include <stdio.h>
#include <stdlib.h>

#include <sool/object.h>
#include <sool/list.h>


int main() {
	list_t *l = list(1,2,3,4,5);
	iterator_t *i;

	forall(i, l)
		printf("%d ", (int)get(i));

	return EXIT_SUCCESS;
}

