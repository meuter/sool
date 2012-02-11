#include <sool/mem.h>
#include <string.h>
#include <assert.h>

#define __USE_GC__

#ifdef __USE_GC__

#include <gc/gc.h>
#define MALLOC  GC_MALLOC
#define FREE    GC_FREE
#define REALLOC GC_REALLOC

#else

#define MALLOC  malloc
#define FREE    free
#define REALLOC realloc

#endif

void *xmalloc(size_t size) {
	void *result = MALLOC(size);
	assert(result);
	return result;
}

void xfree(void *ptr) {
	FREE(ptr);
}

void *xrealloc(void *ptr, size_t size) {
	void *result = REALLOC(ptr, size);
	assert(result);
	return result;
}

char *xstrndup(const char *s, size_t n) {
	char *result = xmalloc(n);
	return strncpy(result, s, n);
}

char *xstrdup(const char *s) {
	int n = strlen(s);
	char *result = xmalloc(n);
	return strncpy(result, s, n);
}
