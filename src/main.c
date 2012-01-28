#include <stdio.h>
#include <stdlib.h>

#include <sool/object.h>
#include <sool/sequence.h>


int main() {
	iterator_class_t *SomeIterator = new(IteratorClass(), "SomeIterator", Object(), 15,
		next, 0x1515,
		get, 0x1616,
		NULL
	);

	oprintf("%O\n", SomeIterator);

	return EXIT_SUCCESS;
}

