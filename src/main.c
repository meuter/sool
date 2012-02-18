#include <sool/list.h>
#include <sool/io.h>
#include <sool/string.h>
#include <printf.h>

int main(void) {
	list_t *l = list("toto", "fofo", "blabla");
	list_t *n = list(1,2,3,4);
	list_t *m = list(l,l,l,l);

	oprintf("hello world\n");
	oprintf("%d\n", 10);
	oprintf("hello %@{%s}\n", l);
	oprintf("hello %@{%s} %d\n", l, 15);
	oprintf("%d hello %d\n", 60, 69);
	oprintf("%d hello %@\n", 60, l);
	oprintf("hello %@{%s} %d %@{%O{%s}}\n", l,15, m);
}



