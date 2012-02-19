#include <sool/list.h>
#include <sool/io.h>
#include <sool/string.h>
#include <sool/dict.h>
#include <stdlib.h>

unsigned long hash(unsigned char *str)
{
	unsigned long hash = 5381;
	int c;

	while ( (c = *str++) )
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

int main(void) {
	dict_t *dict = new(Dictionary(), hash, 2, "hello", 1, "foo", 2);

	dict_put(dict, "hello", (void*)4);
	dict_put(dict, "bar", (void *)15);
	print("%O{%s: %d}\n", (void *)dict);

	return EXIT_SUCCESS;
}



