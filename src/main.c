#include <stdio.h>
#include <stdlib.h>

#include <sool/object.h>

int main() {
	object_t *o = new(Object());
	delete(o);
	return EXIT_SUCCESS;
}

